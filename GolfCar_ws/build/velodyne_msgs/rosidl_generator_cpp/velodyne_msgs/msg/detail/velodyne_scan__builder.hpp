// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from velodyne_msgs:msg/VelodyneScan.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__BUILDER_HPP_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "velodyne_msgs/msg/detail/velodyne_scan__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace velodyne_msgs
{

namespace msg
{

namespace builder
{

class Init_VelodyneScan_packets
{
public:
  explicit Init_VelodyneScan_packets(::velodyne_msgs::msg::VelodyneScan & msg)
  : msg_(msg)
  {}
  ::velodyne_msgs::msg::VelodyneScan packets(::velodyne_msgs::msg::VelodyneScan::_packets_type arg)
  {
    msg_.packets = std::move(arg);
    return std::move(msg_);
  }

private:
  ::velodyne_msgs::msg::VelodyneScan msg_;
};

class Init_VelodyneScan_header
{
public:
  Init_VelodyneScan_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VelodyneScan_packets header(::velodyne_msgs::msg::VelodyneScan::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_VelodyneScan_packets(msg_);
  }

private:
  ::velodyne_msgs::msg::VelodyneScan msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::velodyne_msgs::msg::VelodyneScan>()
{
  return velodyne_msgs::msg::builder::Init_VelodyneScan_header();
}

}  // namespace velodyne_msgs

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__BUILDER_HPP_
