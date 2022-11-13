#pragma once

#include <memory>
#include <string>
#include <vector>

USERVER_NAMESPACE_BEGIN

namespace storages::etcd {

class Component {
 public:
  std::pair<std::string, std::string> KeyValue() const noexcept {
    return std::make_pair(key_, value_);
  }

  std::string GetKey() const noexcept {
    return key_;
  }

  std::string GetValue() const noexcept {
    return value_;
  }

 private:
  std::string key_;
  std::string value_;
};

class Request {
 public:
  std::vector<Component> Get() const noexcept {
    return components_;
  }

 private:
  std::vector<Component> components_;
};

}  // namespace storages::etcd

USERVER_NAMESPACE_END