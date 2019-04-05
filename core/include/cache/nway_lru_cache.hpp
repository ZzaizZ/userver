#pragma once

#include <functional>
#include <vector>

#include <boost/optional.hpp>

#include <cache/lru_cache.hpp>
#include <engine/mutex.hpp>

namespace cache {

template <typename T, typename U>
class NWayLRU {
 public:
  NWayLRU(size_t ways, size_t max_size);

  void Put(const T& key, U value);

  boost::optional<U> Get(const T& key);

  U GetOr(const T& key, const U& default_value);

  void Invalidate();

  template <typename Function>
  void VisitAll(Function func) const;

 private:
  struct Way {
    mutable engine::Mutex mutex;
    // max_size is not used, will be reset by Resize() in NWayLRU::NWayLRU
    LRU<T, U> cache{1};
  };

  Way& GetWay(const T& key);

  std::vector<Way> caches_;
  std::hash<T> hash_fn_;
};

template <typename T, typename U>
NWayLRU<T, U>::NWayLRU(size_t ways, size_t max_size) : caches_(ways) {
  if (ways == 0) throw std::logic_error("Ways must be positive");

  for (auto& way : caches_) way.cache.Resize(max_size);
}

template <typename T, typename U>
void NWayLRU<T, U>::Put(const T& key, U value) {
  auto& way = GetWay(key);

  std::unique_lock<engine::Mutex> lock(way.mutex);
  way.cache.Put(key, std::move(value));
}

template <typename T, typename U>
boost::optional<U> NWayLRU<T, U>::Get(const T& key) {
  auto& way = GetWay(key);
  std::unique_lock<engine::Mutex> lock(way.mutex);
  auto* value = way.cache.Get(key);
  if (value)
    return *value;
  else
    return boost::none;
}

template <typename T, typename U>
U NWayLRU<T, U>::GetOr(const T& key, const U& default_value) {
  auto& way = GetWay(key);
  std::unique_lock<engine::Mutex> lock(way.mutex);
  return way.cache.GetOr(key, default_value);
}

template <typename T, typename U>
void NWayLRU<T, U>::Invalidate() {
  for (auto& way : caches_) {
    std::unique_lock<engine::Mutex> lock(way.mutex);
    way.cache.Invalidate();
  }
}

template <typename T, typename U>
template <typename Function>
void NWayLRU<T, U>::VisitAll(Function func) const {
  for (const auto& way : caches_) {
    std::unique_lock<engine::Mutex> lock(way.mutex);
    way.cache.VisitAll(func);
  }
}

template <typename T, typename U>
typename NWayLRU<T, U>::Way& NWayLRU<T, U>::GetWay(const T& key) {
  auto n = hash_fn_(key) % caches_.size();
  return caches_[n];
}

}  // namespace cache
