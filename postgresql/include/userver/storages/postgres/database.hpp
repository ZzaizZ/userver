#pragma once

/// @file userver/storages/postgres/database.hpp
/// @brief @copybrief storages::postgres::Database

#include <vector>

#include <userver/storages/postgres/options.hpp>
#include <userver/storages/postgres/postgres_fwd.hpp>

namespace components {

class Postgres;

}  // namespace components

namespace storages::postgres {

/// Object for accessing PostgreSQL database instance (sharded or not)
class Database {
 public:
  /// Cluster accessor for default shard number
  ClusterPtr GetCluster() const;

  /// Cluster accessor for specific shard number
  ClusterPtr GetClusterForShard(size_t shard) const;

  /// Get total shard count
  size_t GetShardCount() const { return clusters_.size(); }

 private:
  friend class components::Postgres;
  std::vector<storages::postgres::ClusterPtr> clusters_;
};
using DatabasePtr = std::shared_ptr<Database>;

}  // namespace storages::postgres