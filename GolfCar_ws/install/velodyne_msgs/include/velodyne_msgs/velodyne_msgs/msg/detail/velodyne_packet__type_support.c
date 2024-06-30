// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "velodyne_msgs/msg/detail/velodyne_packet__rosidl_typesupport_introspection_c.h"
#include "velodyne_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "velodyne_msgs/msg/detail/velodyne_packet__functions.h"
#include "velodyne_msgs/msg/detail/velodyne_packet__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  velodyne_msgs__msg__VelodynePacket__init(message_memory);
}

void velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_fini_function(void * message_memory)
{
  velodyne_msgs__msg__VelodynePacket__fini(message_memory);
}

size_t velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__size_function__VelodynePacket__data(
  const void * untyped_member)
{
  (void)untyped_member;
  return 1206;
}

const void * velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_const_function__VelodynePacket__data(
  const void * untyped_member, size_t index)
{
  const uint8_t * member =
    (const uint8_t *)(untyped_member);
  return &member[index];
}

void * velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_function__VelodynePacket__data(
  void * untyped_member, size_t index)
{
  uint8_t * member =
    (uint8_t *)(untyped_member);
  return &member[index];
}

void velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__fetch_function__VelodynePacket__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const uint8_t * item =
    ((const uint8_t *)
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_const_function__VelodynePacket__data(untyped_member, index));
  uint8_t * value =
    (uint8_t *)(untyped_value);
  *value = *item;
}

void velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__assign_function__VelodynePacket__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  uint8_t * item =
    ((uint8_t *)
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_function__VelodynePacket__data(untyped_member, index));
  const uint8_t * value =
    (const uint8_t *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_member_array[2] = {
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(velodyne_msgs__msg__VelodynePacket, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    1206,  // array size
    false,  // is upper bound
    offsetof(velodyne_msgs__msg__VelodynePacket, data),  // bytes offset in struct
    NULL,  // default value
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__size_function__VelodynePacket__data,  // size() function pointer
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_const_function__VelodynePacket__data,  // get_const(index) function pointer
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__get_function__VelodynePacket__data,  // get(index) function pointer
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__fetch_function__VelodynePacket__data,  // fetch(index, &value) function pointer
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__assign_function__VelodynePacket__data,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_members = {
  "velodyne_msgs__msg",  // message namespace
  "VelodynePacket",  // message name
  2,  // number of fields
  sizeof(velodyne_msgs__msg__VelodynePacket),
  velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_member_array,  // message members
  velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_init_function,  // function to initialize message memory (memory has to be allocated)
  velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_type_support_handle = {
  0,
  &velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_velodyne_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, velodyne_msgs, msg, VelodynePacket)() {
  velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_type_support_handle.typesupport_identifier) {
    velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &velodyne_msgs__msg__VelodynePacket__rosidl_typesupport_introspection_c__VelodynePacket_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
