#pragma once
#include <rclcpp/rclcpp.hpp>
#include "manager/node_manager.hpp"

namespace robosense::lidar
{
class RslidarSdkComponent : public rclcpp::Node
{
public:
  explicit RslidarSdkComponent(const rclcpp::NodeOptions & options);

private:
  std::shared_ptr<NodeManager> node_mgr_;
};
} // namespace robosense::lidar
