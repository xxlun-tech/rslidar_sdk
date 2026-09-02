#include "rslidar_sdk_component.hpp"

#include <rs_driver/macro/version.hpp>
#include <yaml-cpp/yaml.h>

#include <ament_index_cpp/get_package_share_directory.hpp>

namespace robosense::lidar
{
RslidarSdkComponent::RslidarSdkComponent(const rclcpp::NodeOptions & options)
: Node("rslidar_sdk_component", options)
{
  RS_TITLE << "********************************************************" << RS_REND;
  RS_TITLE << "**********                                    **********" << RS_REND;
  RS_TITLE << "**********    RSLidar_SDK Version: v" << RSLIDAR_VERSION_MAJOR 
    << "." << RSLIDAR_VERSION_MINOR 
    << "." << RSLIDAR_VERSION_PATCH << "     **********" << RS_REND;
  RS_TITLE << "**********                                    **********" << RS_REND;
  RS_TITLE << "********************************************************" << RS_REND;

  std::string pkg_share = ament_index_cpp::get_package_share_directory("rslidar_sdk");
  std::string default_config_path = pkg_share + "/config/config.yaml";
  std::string config_path = this->declare_parameter<std::string>("config_path", default_config_path);

  YAML::Node config;
  try
  {
    config = YAML::LoadFile(config_path);
    RS_INFO << "--------------------------------------------------------" << RS_REND;
    RS_INFO << "Config loaded from PATH:" << RS_REND;
    RS_INFO << config_path << RS_REND;
    RS_INFO << "--------------------------------------------------------" << RS_REND;
  }
  catch (...)
  {
    RS_ERROR << "The format of config file " << config_path << " is wrong. Please check (e.g. indentation)." << RS_REND;
    return;
  }

  // 初始化 NodeManager
  node_mgr_ = std::make_shared<NodeManager>();
  node_mgr_->init(config);
  node_mgr_->start();

  RS_MSG << "RoboSense-LiDAR-Driver(component) is running....." << RS_REND;
}
} // namespace robosense::lidar

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(robosense::lidar::RslidarSdkComponent)
