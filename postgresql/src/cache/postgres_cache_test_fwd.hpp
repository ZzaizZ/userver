/*! [Pg Cache Fwd Example] */
#pragma once

#include <memory>  // for std::shared_ptr

#include <cache/base_postgres_cache_fwd.hpp>

namespace example {  // replace with a namespace of your trait

struct PostgresExamplePolicy;
struct MyStructure;

}  // namespace example

namespace caches {

using MyCache1 = components::PostgreCache<example::PostgresExamplePolicy>;
using MyCache1Data = std::shared_ptr<const example::MyStructure>;

}  // namespace caches
/*! [Pg Cache Fwd Example] */