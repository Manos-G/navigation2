// Copyright (c) 2023 Samsung R&D Institute Russia
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef NAV2_MAP_SERVER__VECTOR_OBJECT_SHAPES_HPP_
#define NAV2_MAP_SERVER__VECTOR_OBJECT_SHAPES_HPP_

#include <cmath>
#include <memory>
#include <string>
#include <vector>

#include "geometry_msgs/msg/polygon.hpp"
#include "geometry_msgs/msg/point32.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"

#include "tf2_ros/buffer.h"

#include "nav2_msgs/msg/polygon_vo.hpp"
#include "nav2_msgs/msg/circle_vo.hpp"
#include "nav2_util/lifecycle_node.hpp"

#include "nav2_map_server/vector_object_utils.hpp"

namespace nav2_map_server
{

/// @brief Possible VO-shape types
enum ShapeType
{
  UNKNOWN = 0,
  POLYGON = 1,
  CIRCLE = 2
};

/// @brief Basic class, other vector objects to be inherited from
class Shape
{
public:
  /**
   * @brief Shape basic class constructor
   * @param node Vector Object server node pointer
   */
  explicit Shape(const nav2_util::LifecycleNode::WeakPtr & node);

  /**
   * @brief Shape destructor
   */
  virtual ~Shape();

  /**
   * @brief Returns type of the shape
   * @return Type of the shape
   */
  ShapeType getType();

  /**
   * @brief Supporting routine obtaining shape UUID from ROS-parameters
   * for the given shape object
   * @return True if UUID was obtained or false in failure case
   */
  bool getShapeUUID(const std::string & shape_name, unsigned char * out_uuid);

  /**
   * @brief Supporting routine obtaining ROS-parameters for the given vector object.
   * Empty virtual method intended to be used in child implementations
   * @param shape_name Name of the shape
   * @return True if all parameters were obtained or false in failure case
   */
  virtual bool getROSParameters(const std::string & shape_name) = 0;

  /**
   * @brief Gets shape boundaries.
   * Empty virtual method intended to be used in child implementations
   * @param min_x output min X-boundary of shape
   * @param min_y output min Y-boundary of shape
   * @param max_x output max X-boundary of shape
   * @param max_y output max Y-boundary of shape
   */
  virtual void getBoundaries(double & min_x, double & min_y, double & max_x, double & max_y) = 0;

  /**
   * @brief Is the point inside the shape.
   * Empty virtual method intended to be used in child implementations
   * @param px X-coordinate of the given point to check
   * @param py Y-coordinate of the given point to check
   * @return True if given point inside the shape
   */
  virtual bool isPointInside(const double px, const double py) const = 0;

  /**
   * @brief Puts shape borders on map.
   * Empty virtual method intended to be used in child implementations
   * @param map Output map pointer
   * @param overlay_type Overlay type
   */
  virtual void putBorders(
    nav_msgs::msg::OccupancyGrid::SharedPtr map, const OverlayType overlay_type) = 0;

  /**
   * @brief Gets the value of the shape.
   * Empty virtual method intended to be used in child implementations
   * @return OccupancyGrid value of the shape
   */
  virtual int8_t getValue() const = 0;

  /**
   * @brief Gets frame ID of the shape.
   * Empty virtual method intended to be used in child implementations
   * @return Frame ID where the shape is
   */
  virtual std::string getFrameID() const = 0;

  /**
   * @brief Gets UUID of the shape.
   * Empty virtual method intended to be used in child implementations
   * @return Shape UUID string
   */
  virtual std::string getUUID() const = 0;

  /**
   * @brief Checks whether the shape is equal to a given UUID.
   * Empty virtual method intended to be used in child implementations
   * @param uuid Given UUID to check with
   * @return True if the shape has the same as given UUID, otherwise false
   */
  virtual bool isUUID(const unsigned char * uuid) const = 0;

  /**
   * @brief Whether the shape to be filled or only its borders to be put on map.
   * Empty virtual method intended to be used in child implementations
   * @return True if shape to be filled
   */
  virtual bool isFill() const = 0;

  /**
   * @brief Transforms shape coordinates to a new frame.
   * Empty virtual method intended to be used in child implementations
   * @param to_frame Frame ID to transform to
   * @param tf_buffer TF buffer to use for the transformation
   * @param transform_tolerance Transform tolerance
   * @return Whether it was transformed successfully
   */
  virtual bool toFrame(
    const std::string & to_frame,
    const std::shared_ptr<tf2_ros::Buffer> tf_buffer,
    const double transform_tolerance) = 0;

protected:
  /// @brief Type of shape
  ShapeType type_;

  /// @brief VectorObjectServer node
  nav2_util::LifecycleNode::WeakPtr node_;
};

/// @brief Polygon shape class
class Polygon : public Shape
{
public:
  /*
   * @brief Polygon class constructor
   * @param node Vector Object server node pointer
   * @param params PolygonVO parameters. In case of nullptr,
   * parameters to be read from ROS-parameters.
   * @throw std::exception in case of inconsistent shape
   */
  Polygon(
    const nav2_util::LifecycleNode::WeakPtr & node,
    const nav2_msgs::msg::PolygonVO::SharedPtr params = nullptr);

  /**
   * @brief Supporting routine obtaining ROS-parameters for the given vector object.
   * @param shape_name Name of the shape
   * @return True if all parameters were obtained or false in failure case
   */
  bool getROSParameters(const std::string & shape_name);

  /**
   * @brief Gets shape boundaries
   * @param min_x output min X-boundary of shape
   * @param min_y output min Y-boundary of shape
   * @param max_x output max X-boundary of shape
   * @param max_y output max Y-boundary of shape
   */
  void getBoundaries(double & min_x, double & min_y, double & max_x, double & max_y);

  /**
   * @brief Is the point inside the shape.
   * @param px X-coordinate of the given point to check
   * @param py Y-coordinate of the given point to check
   * @return True if given point inside the shape
   */
  bool isPointInside(const double px, const double py) const;

  /**
   * @brief Puts shape borders on map.
   * @param map Output map pointer
   * @param overlay_type Overlay type
   */
  void putBorders(nav_msgs::msg::OccupancyGrid::SharedPtr map, const OverlayType overlay_type);

  /**
   * @brief Gets Polygon parameters
   * @return Polygon parameters
   */
  nav2_msgs::msg::PolygonVO::SharedPtr getParams() const;

  /**
   * @brief Tries to update Polygon parameters
   * @throw std::exception in case of inconsistent shape
   */
  void setParams(const nav2_msgs::msg::PolygonVO::SharedPtr params);

  /**
   * @brief Gets the value of the shape.
   * @return OccupancyGrid value of the shape
   */
  int8_t getValue() const;

  /**
   * @brief Gets frame ID of the shape.
   * @return Frame ID where the shape is
   */
  std::string getFrameID() const;

  /**
   * @brief Gets UUID of the shape.
   * @return Shape UUID string
   */
  std::string getUUID() const;

  /**
   * @brief Checks whether the shape is equal to a given UUID.
   * @param uuid Given UUID to check with
   * @return True if the shape has the same as given UUID, otherwise false
   */
  bool isUUID(const unsigned char * uuid) const;

  /**
   * @brief Whether the shape to be filled or only its borders to be put on map.
   * @return True if shape to be filled
   */
  bool isFill() const;

  /**
   * @brief Transforms shape coordinates to a new frame.
   * @param to_frame Frame ID to transform to
   * @param tf_buffer TF buffer to use for the transformation
   * @param transform_tolerance Transform tolerance
   * @return Whether it was transformed successfully
   */
  bool toFrame(
    const std::string & to_frame,
    const std::shared_ptr<tf2_ros::Buffer> tf_buffer,
    const double transform_tolerance);

protected:
  /**
   * @brief Checks that shape is consistent for further operation
   * @throw std::exception in case of inconsistent shape
   */
  void checkConsistency();

  /// @brief Input polygon parameters (could be in any frame)
  nav2_msgs::msg::PolygonVO::SharedPtr params_;
  /// @brief Polygon in the map's frame
  geometry_msgs::msg::Polygon::SharedPtr polygon_;
};

/// @brief Circle shape class
class Circle : public Shape
{
public:
  /*
   * @brief Circle class constructor
   * @param node Vector Object server node pointer
   * @param params CircleVO parameters. In case of nullptr,
   * parameters to be read from ROS-parameters.
   * @throw std::exception in case of inconsistent shape
   */
  Circle(
    const nav2_util::LifecycleNode::WeakPtr & node,
    const nav2_msgs::msg::CircleVO::SharedPtr params = nullptr);

  /**
   * @brief Supporting routine obtaining ROS-parameters for the given vector object.
   * @param shape_name Name of the shape
   * @return True if all parameters were obtained or false in failure case
   */
  bool getROSParameters(const std::string & shape_name);

  /**
   * @brief Gets shape boundaries
   * @param min_x output min X-boundary of shape
   * @param min_y output min Y-boundary of shape
   * @param max_x output max X-boundary of shape
   * @param max_y output max Y-boundary of shape
   */
  void getBoundaries(double & min_x, double & min_y, double & max_x, double & max_y);

  /**
   * @brief Is the point inside the shape.
   * @param px X-coordinate of the given point to check
   * @param py Y-coordinate of the given point to check
   * @return True if given point inside the shape
   */
  bool isPointInside(const double px, const double py) const;

  /**
   * @brief Puts shape borders on map.
   * @param map Output map pointer
   * @param overlay_type Overlay type
   */
  void putBorders(nav_msgs::msg::OccupancyGrid::SharedPtr map, const OverlayType overlay_type);

  /**
   * @brief Gets Circle parameters
   * @return Circle parameters
   */
  nav2_msgs::msg::CircleVO::SharedPtr getParams() const;

  /**
   * @brief Tries to update Circle parameters
   * @throw std::exception in case of inconsistent shape
   */
  void setParams(const nav2_msgs::msg::CircleVO::SharedPtr params);

  /**
   * @brief Gets the value of the shape.
   * @return OccupancyGrid value of the shape
   */
  int8_t getValue() const;

  /**
   * @brief Gets frame ID of the shape.
   * @return Frame ID where the shape is
   */
  std::string getFrameID() const;

  /**
   * @brief Gets UUID of the shape.
   * @return Shape UUID string
   */
  std::string getUUID() const;

  /**
   * @brief Checks whether the shape is equal to a given UUID.
   * @param uuid Given UUID to check with
   * @return True if the shape has the same as given UUID, otherwise false
   */
  bool isUUID(const unsigned char * uuid) const;

  /**
   * @brief Whether the shape to be filled or only its borders to be put on map.
   * @return True if shape to be filled
   */
  bool isFill() const;

  /**
   * @brief Transforms shape coordinates to a new frame.
   * @param to_frame Frame ID to transform to
   * @param tf_buffer TF buffer to use for the transformation
   * @param transform_tolerance Transform tolerance
   * @return Whether it was transformed successfully
   */
  bool toFrame(
    const std::string & to_frame,
    const std::shared_ptr<tf2_ros::Buffer> tf_buffer,
    const double transform_tolerance);

protected:
  /**
   * @brief Checks that shape is consistent for further operation
   * @throw std::exception in case of inconsistent shape
   */
  void checkConsistency();

  /**
   * @brief Converts circle center to map coordinates
   * considering FP-accuracy loosing on small values when using conventional
   * nav2_util::worldToMap() approach
   * @param map Map pointer
   * @param mcx Output X-coordinate of associated circle center on map
   * @param mcy Output Y-coordinate of associated circle center on map
   * @return True if the conversion was successful
   */
  bool centerToMap(
    nav_msgs::msg::OccupancyGrid::ConstSharedPtr map,
    unsigned int & mcx, unsigned int & mcy);

  /**
   * @brief Put Circle's point on map
   * @param mx X-coordinate of given point
   * @param my Y-coordinate of given point
   * @param map Output map pointer
   * @param overlay_type Overlay type
   */
  void putPoint(
    unsigned int mx, unsigned int my,
    nav_msgs::msg::OccupancyGrid::SharedPtr map,
    const OverlayType overlay_type);

  /// @brief Input circle parameters (could be in any frame)
  nav2_msgs::msg::CircleVO::SharedPtr params_;
  /// @brief Circle center in the map's frame
  geometry_msgs::msg::Point32::SharedPtr center_;
};

/// @brief Functor class used in raytraceLine algorithm
class MapAction
{
public:
  /**
   * @brief MapAction constructor
   * @param map Output map pointer
   * @param value Value to put on map
   * @param overlay_type Overlay type
   */
  MapAction(
    nav_msgs::msg::OccupancyGrid::SharedPtr map,
    int8_t value, OverlayType overlay_type)
  : map_(map), value_(value), overlay_type_(overlay_type)
  {}

  /**
   * @brief Map filling operator
   * @param offset Offset on the map where the cell to be changed
   */
  inline void operator()(unsigned int offset)
  {
    fillMap(map_, offset, value_, overlay_type_);
  }

protected:
  /// @brief Output map pointer
  nav_msgs::msg::OccupancyGrid::SharedPtr map_;
  /// @brief Value to put on map
  int8_t value_;
  /// @brief Overlay type
  OverlayType overlay_type_;
};

}  // namespace nav2_map_server

#endif  // NAV2_MAP_SERVER__VECTOR_OBJECT_SHAPES_HPP_
