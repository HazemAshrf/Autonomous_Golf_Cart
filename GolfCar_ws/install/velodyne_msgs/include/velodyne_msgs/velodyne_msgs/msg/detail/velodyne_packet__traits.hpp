// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__TRAITS_HPP_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "velodyne_msgs/msg/detail/velodyne_packet__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace velodyne_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const VelodynePacket & msg,
  std::ostream & out)
{
  out << "{";
  // member: stamp
  {
    out << "stamp: ";
    to_flow_style_yaml(msg.stamp, out);
    out << ", ";
  }

  // member: data
  {
    if (msg.data.size() == 0) {
      out << "data: []";
    } else {
      out << "data: [";
      size_t pending_items = msg.data.size();
      for (auto item : msg.data) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const VelodynePacket & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: stamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stamp:\n";
    to_block_style_yaml(msg.stamp, out, indentation + 2);
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.data.size() == 0) {
      out << "data: []\n";
    } else {
      out << "data:\n";
      for (auto item : msg.data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const VelodynePacket & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace velodyne_msgs

namespace rosidl_generator_traits
{

[[deprecated("use velodyne_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const velodyne_msgs::msg::VelodynePacket & msg,
  std::ostream & out, size_t indentation = 0)
{
  velodyne_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use velodyne_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const velodyne_msgs::msg::VelodynePacket & msg)
{
  return velodyne_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<velodyne_msgs::msg::VelodynePacket>()
{
  return "velodyne_msgs::msg::VelodynePacket";
}

template<>
inline const char * name<velodyne_msgs::msg::VelodynePacket>()
{
  return "velodyne_msgs/msg/VelodynePacket";
}

template<>
struct has_fixed_size<velodyne_msgs::msg::VelodynePacket>
  : std::integral_constant<bool, has_fixed_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct has_bounded_size<velodyne_msgs::msg::VelodynePacket>
  : std::integral_constant<bool, has_bounded_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct is_message<velodyne_msgs::msg::VelodynePacket>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__TRAITS_HPP_
