cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)
project(rslidar_sdk)

#=======================================
# Custom Point Type (XYZI,XYZIRT, XYZIF, XYZIRTF, XYZIRCAEDT)
#=======================================
set(POINT_TYPE XYZIRCAEDT)

option(ENABLE_TRANSFORM "Enable transform functions" OFF)
if(${ENABLE_TRANSFORM})
  add_definitions("-DENABLE_TRANSFORM")
  find_package(Eigen3 REQUIRED)
  include_directories(${EIGEN3_INCLUDE_DIR})
endif(${ENABLE_TRANSFORM})

option(ENABLE_EPOLL_RECEIVE "Receive packets with epoll() instead of select()" OFF)
if(${ENABLE_EPOLL_RECEIVE})
  add_definitions("-DENABLE_EPOLL_RECEIVE")
endif(${ENABLE_EPOLL_RECEIVE})

option(ENABLE_MODIFY_RECVBUF "Enable modify size of RECVBUF" ON)
if(${ENABLE_MODIFY_RECVBUF})
  add_definitions("-DENABLE_MODIFY_RECVBUF")
endif(${ENABLE_MODIFY_RECVBUF})

option(ENABLE_WAIT_IF_QUEUE_EMPTY "Enable waiting for a while in handle thread if the queue is empty" OFF)
if(${ENABLE_WAIT_IF_QUEUE_EMPTY})
  add_definitions("-DENABLE_WAIT_IF_QUEUE_EMPTY")
endif(${ENABLE_WAIT_IF_QUEUE_EMPTY})

option(ENABLE_DIFOP_PARSE "Enable parsing DIFOP Packet" ON)
if(${ENABLE_DIFOP_PARSE})
  add_definitions("-DENABLE_DIFOP_PARSE")
endif(${ENABLE_DIFOP_PARSE})

option(ENABLE_STAMP_WITH_LOCAL "Enable stamp point cloud with local time" OFF)
if(${ENABLE_STAMP_WITH_LOCAL})
  add_definitions("-DENABLE_STAMP_WITH_LOCAL")
endif(${ENABLE_STAMP_WITH_LOCAL})


option(ENABLE_SOURCE_PACKET_LEGACY "Enable ROS Source of MSOP/DIFOP Packet v1.3.x" OFF)
if(${ENABLE_SOURCE_PACKET_LEGACY})
  add_definitions("-DENABLE_SOURCE_PACKET_LEGACY")
endif(${ENABLE_SOURCE_PACKET_LEGACY})

option(ENABLE_IMU_DATA_PARSE           "Enable imu data parse" OFF)
if(${ENABLE_IMU_DATA_PARSE})

  message(=============================================================)
  message("-- Enable imu data parse")
  message(=============================================================)

  add_definitions("-DENABLE_IMU_DATA_PARSE")

endif(${ENABLE_IMU_DATA_PARSE})

#========================
# Project details / setup
#========================
set(PROJECT_NAME rslidar_sdk)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall)

#========================
# Point Type Definition
#========================
if(${POINT_TYPE} STREQUAL "XYZI")
  add_definitions(-DPOINT_TYPE_XYZI)
elseif(${POINT_TYPE} STREQUAL "XYZIRT")
  add_definitions(-DPOINT_TYPE_XYZIRT)
elseif(${POINT_TYPE} STREQUAL "XYZIF")
  add_definitions(-DPOINT_TYPE_XYZIF)
elseif(${POINT_TYPE} STREQUAL "XYZIRTF")
  add_definitions(-DPOINT_TYPE_XYZIRTF)
elseif(${POINT_TYPE} STREQUAL "XYZIRCAEDT")
  add_definitions(-DPOINT_TYPE_XYZIRCAEDT)
endif()

message(=============================================================)
message("-- POINT_TYPE is ${POINT_TYPE}")
message(=============================================================)

#========================
# Dependencies Setup
#========================

find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()

#Others#
find_package(yaml-cpp REQUIRED)

#Driver core#
add_subdirectory(src/rs_driver)
find_package(rs_driver REQUIRED)

#========================
# Build Setup
#========================

ament_auto_add_library(${PROJECT_NAME} SHARED
  src/node/rslidar_sdk_component.cpp
  src/manager/node_manager.cpp
)
target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${rs_driver_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
  ${YAML_CPP_LIBRARIES}
  ${rs_driver_LIBRARIES}
)

ament_target_dependencies(${PROJECT_NAME}
  rclcpp
  rclcpp_components
  std_msgs
  sensor_msgs
  rslidar_msg
  ament_index_cpp
)

rclcpp_components_register_node(${PROJECT_NAME}
  PLUGIN "robosense::lidar::RslidarSdkComponent"
  EXECUTABLE ${PROJECT_NAME}_node
)

ament_auto_package(
  INSTALL_TO_SHARE
  config
  launch
)
