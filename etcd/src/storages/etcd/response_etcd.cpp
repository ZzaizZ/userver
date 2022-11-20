#include <userver/storages/etcd/response_etcd.hpp>

USERVER_NAMESPACE_BEGIN

namespace storages::etcd {

KeyValue::KeyValue(const std::string& key, const std::string& value)
    : key_(key), value_(value) {}

std::pair<std::string, std::string> KeyValue::GetPair() const {
  return std::make_pair(key_, value_);
}

std::string KeyValue::GetKey() const { return key_; }

std::string KeyValue::GetValue() const { return value_; }

std::vector<KeyValue> Response::Get() const { return key_values_; }

Response::Response(const std::vector<KeyValue>& key_values)
    : key_values_(key_values) {}

}  // namespace storages::etcd

USERVER_NAMESPACE_END