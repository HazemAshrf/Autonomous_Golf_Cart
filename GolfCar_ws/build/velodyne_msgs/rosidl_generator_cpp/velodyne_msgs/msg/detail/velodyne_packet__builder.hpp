// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__BUILDER_HPP_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "velodyne_msgs/msg/detail/velodyne_packet__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace velodyne_msgs
{

namespace msg
{

namespace builder
{

class Init_VelodynePacket_data
{
public:
  explicit Init_VelodynePacket_data(::velodyne_msgs::msg::VelodynePacket & msg)
  : msg_(msg)
  {}
  ::velodyne_msgs::msg::VelodynePacket data(::velodyne_msgs::msg::VelodynePacket::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::velodyne_msgs::msg::VelodynePacket msg_;
};

class Init_VelodynePacket_stamp
{
public:
  Init_VelodynePacket_stamp()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_VelodynePacket_data stamp(::velodyne_msgs::msg::VelodynePacket::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return Init_VelodynePacket_data(msg_);
  }

private:
  ::velodyne_msgs::msg::VelodynePacket msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::velodyne_msgs::msg::VelodynePacket>()
{
  return velodyne_msgs::msg::builder::Init_VelodynePacket_stamp();
}

}  // namespace velodyne_msgs

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__BUILDER_HPP_
