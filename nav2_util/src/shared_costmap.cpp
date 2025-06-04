#include "nav2_util/shared_costmap.hpp"

namespace nav2_util
{

std::shared_ptr<nav2_costmap_2d::Costmap2DROS> shared_costmap = nullptr;
std::unique_ptr<nav2_util::NodeThread> shared_costmap_thread;
bool shared_costmap_initialized = false;
std::string shared_costmap_name;

void init_shared_costmap(
  const std::string & parent_namespace,
  const std::string & costmap_name)
{
  if (!shared_costmap) {
    shared_costmap_name = costmap_name;
    shared_costmap = std::make_shared<nav2_costmap_2d::Costmap2DROS>(
      costmap_name, parent_namespace, costmap_name);
  }
}

}  // namespace nav2_util
