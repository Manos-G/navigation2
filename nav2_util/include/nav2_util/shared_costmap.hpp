#ifndef NAV2_UTIL__SHARED_COSTMAP_HPP_
#define NAV2_UTIL__SHARED_COSTMAP_HPP_

#include <memory>
#include "nav2_costmap_2d/costmap_2d_ros.hpp"
#include "nav2_util/node_thread.hpp"

namespace nav2_util
{

extern std::shared_ptr<nav2_costmap_2d::Costmap2DROS> shared_costmap;
extern std::unique_ptr<nav2_util::NodeThread> shared_costmap_thread;
extern bool shared_costmap_initialized;

void init_shared_costmap(const std::string & parent_namespace);

}  // namespace nav2_util

#endif  // NAV2_UTIL__SHARED_COSTMAP_HPP_
