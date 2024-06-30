// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from velodyne_msgs:msg/VelodyneScan.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_H_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'packets'
#include "velodyne_msgs/msg/detail/velodyne_packet__struct.h"

/// Struct defined in msg/VelodyneScan in the package velodyne_msgs.
/**
  * Velodyne LIDAR scan packets.
 */
typedef struct velodyne_msgs__msg__VelodyneScan
{
  /// standard ROS message header
  std_msgs__msg__Header header;
  /// vector of raw packets
  velodyne_msgs__msg__VelodynePacket__Sequence packets;
} velodyne_msgs__msg__VelodyneScan;

// Struct for a sequence of velodyne_msgs__msg__VelodyneScan.
typedef struct velodyne_msgs__msg__VelodyneScan__Sequence
{
  velodyne_msgs__msg__VelodyneScan * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} velodyne_msgs__msg__VelodyneScan__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_H_
