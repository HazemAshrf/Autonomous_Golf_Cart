// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from velodyne_msgs:msg/VelodyneScan.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "velodyne_msgs/msg/detail/velodyne_scan__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace velodyne_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void VelodyneScan_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) velodyne_msgs::msg::VelodyneScan(_init);
}

void VelodyneScan_fini_function(void * message_memory)
{
  auto typed_message = static_cast<velodyne_msgs::msg::VelodyneScan *>(message_memory);
  typed_message->~VelodyneScan();
}

size_t size_function__VelodyneScan__packets(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<velodyne_msgs::msg::VelodynePacket> *>(untyped_member);
  return member->size();
}

const void * get_const_function__VelodyneScan__packets(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<velodyne_msgs::msg::VelodynePacket> *>(untyped_member);
  return &member[index];
}

void * get_function__VelodyneScan__packets(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<velodyne_msgs::msg::VelodynePacket> *>(untyped_member);
  return &member[index];
}

void fetch_function__VelodyneScan__packets(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const velodyne_msgs::msg::VelodynePacket *>(
    get_const_function__VelodyneScan__packets(untyped_member, index));
  auto & value = *reinterpret_cast<velodyne_msgs::msg::VelodynePacket *>(untyped_value);
  value = item;
}

void assign_function__VelodyneScan__packets(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<velodyne_msgs::msg::VelodynePacket *>(
    get_function__VelodyneScan__packets(untyped_member, index));
  const auto & value = *reinterpret_cast<const velodyne_msgs::msg::VelodynePacket *>(untyped_value);
  item = value;
}

void resize_function__VelodyneScan__packets(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<velodyne_msgs::msg::VelodynePacket> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember VelodyneScan_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(velodyne_msgs::msg::VelodyneScan, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "packets",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<velodyne_msgs::msg::VelodynePacket>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(velodyne_msgs::msg::VelodyneScan, packets),  // bytes offset in struct
    nullptr,  // default value
    size_function__VelodyneScan__packets,  // size() function pointer
    get_const_function__VelodyneScan__packets,  // get_const(index) function pointer
    get_function__VelodyneScan__packets,  // get(index) function pointer
    fetch_function__VelodyneScan__packets,  // fetch(index, &value) function pointer
    assign_function__VelodyneScan__packets,  // assign(index, value) function pointer
    resize_function__VelodyneScan__packets  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers VelodyneScan_message_members = {
  "velodyne_msgs::msg",  // message namespace
  "VelodyneScan",  // message name
  2,  // number of fields
  sizeof(velodyne_msgs::msg::VelodyneScan),
  VelodyneScan_message_member_array,  // message members
  VelodyneScan_init_function,  // function to initialize message memory (memory has to be allocated)
  VelodyneScan_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t VelodyneScan_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &VelodyneScan_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace velodyne_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<velodyne_msgs::msg::VelodyneScan>()
{
  return &::velodyne_msgs::msg::rosidl_typesupport_introspection_cpp::VelodyneScan_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, velodyne_msgs, msg, VelodyneScan)() {
  return &::velodyne_msgs::msg::rosidl_typesupport_introspection_cpp::VelodyneScan_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
