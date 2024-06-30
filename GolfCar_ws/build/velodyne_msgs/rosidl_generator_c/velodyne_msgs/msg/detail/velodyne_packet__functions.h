// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__FUNCTIONS_H_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/visibility_control.h"
#include "velodyne_msgs/msg/rosidl_generator_c__visibility_control.h"

#include "velodyne_msgs/msg/detail/velodyne_packet__struct.h"

/// Initialize msg/VelodynePacket message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * velodyne_msgs__msg__VelodynePacket
 * )) before or use
 * velodyne_msgs__msg__VelodynePacket__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__init(velodyne_msgs__msg__VelodynePacket * msg);

/// Finalize msg/VelodynePacket message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
void
velodyne_msgs__msg__VelodynePacket__fini(velodyne_msgs__msg__VelodynePacket * msg);

/// Create msg/VelodynePacket message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * velodyne_msgs__msg__VelodynePacket__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
velodyne_msgs__msg__VelodynePacket *
velodyne_msgs__msg__VelodynePacket__create();

/// Destroy msg/VelodynePacket message.
/**
 * It calls
 * velodyne_msgs__msg__VelodynePacket__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
void
velodyne_msgs__msg__VelodynePacket__destroy(velodyne_msgs__msg__VelodynePacket * msg);

/// Check for msg/VelodynePacket message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__are_equal(const velodyne_msgs__msg__VelodynePacket * lhs, const velodyne_msgs__msg__VelodynePacket * rhs);

/// Copy a msg/VelodynePacket message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__copy(
  const velodyne_msgs__msg__VelodynePacket * input,
  velodyne_msgs__msg__VelodynePacket * output);

/// Initialize array of msg/VelodynePacket messages.
/**
 * It allocates the memory for the number of elements and calls
 * velodyne_msgs__msg__VelodynePacket__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__Sequence__init(velodyne_msgs__msg__VelodynePacket__Sequence * array, size_t size);

/// Finalize array of msg/VelodynePacket messages.
/**
 * It calls
 * velodyne_msgs__msg__VelodynePacket__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
void
velodyne_msgs__msg__VelodynePacket__Sequence__fini(velodyne_msgs__msg__VelodynePacket__Sequence * array);

/// Create array of msg/VelodynePacket messages.
/**
 * It allocates the memory for the array and calls
 * velodyne_msgs__msg__VelodynePacket__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
velodyne_msgs__msg__VelodynePacket__Sequence *
velodyne_msgs__msg__VelodynePacket__Sequence__create(size_t size);

/// Destroy array of msg/VelodynePacket messages.
/**
 * It calls
 * velodyne_msgs__msg__VelodynePacket__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
void
velodyne_msgs__msg__VelodynePacket__Sequence__destroy(velodyne_msgs__msg__VelodynePacket__Sequence * array);

/// Check for msg/VelodynePacket message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__Sequence__are_equal(const velodyne_msgs__msg__VelodynePacket__Sequence * lhs, const velodyne_msgs__msg__VelodynePacket__Sequence * rhs);

/// Copy an array of msg/VelodynePacket messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_velodyne_msgs
bool
velodyne_msgs__msg__VelodynePacket__Sequence__copy(
  const velodyne_msgs__msg__VelodynePacket__Sequence * input,
  velodyne_msgs__msg__VelodynePacket__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_PACKET__FUNCTIONS_H_
