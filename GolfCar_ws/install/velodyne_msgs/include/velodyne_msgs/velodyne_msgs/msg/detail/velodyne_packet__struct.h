// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__STRUCT_H_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/VelodynePacket in the package velodyne_msgs.
/**
  * Raw Velodyne LIDAR packet.
 */
typedef struct velodyne_msgs__msg__VelodynePacket
{
  /// packet timestamp
  builtin_interfaces__msg__Time stamp;
  /// packet contents
  uint8_t data[1206];
} velodyne_msgs__msg__VelodynePacket;

// Struct for a sequence of velodyne_msgs__msg__VelodynePacket.
typedef struct velodyne_msgs__msg__VelodynePacket__Sequence
{
  velodyne_msgs__msg__VelodynePacket * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} velodyne_msgs__msg__VelodynePacket__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__STRUCT_H_
