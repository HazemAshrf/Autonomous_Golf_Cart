// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from velodyne_msgs:msg/VelodynePacket.idl
// generated code does not contain a copyright notice
#include "velodyne_msgs/msg/detail/velodyne_packet__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
velodyne_msgs__msg__VelodynePacket__init(velodyne_msgs__msg__VelodynePacket * msg)
{
  if (!msg) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    velodyne_msgs__msg__VelodynePacket__fini(msg);
    return false;
  }
  // data
  return true;
}

void
velodyne_msgs__msg__VelodynePacket__fini(velodyne_msgs__msg__VelodynePacket * msg)
{
  if (!msg) {
    return;
  }
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
  // data
}

bool
velodyne_msgs__msg__VelodynePacket__are_equal(const velodyne_msgs__msg__VelodynePacket * lhs, const velodyne_msgs__msg__VelodynePacket * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  // data
  for (size_t i = 0; i < 1206; ++i) {
    if (lhs->data[i] != rhs->data[i]) {
      return false;
    }
  }
  return true;
}

bool
velodyne_msgs__msg__VelodynePacket__copy(
  const velodyne_msgs__msg__VelodynePacket * input,
  velodyne_msgs__msg__VelodynePacket * output)
{
  if (!input || !output) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  // data
  for (size_t i = 0; i < 1206; ++i) {
    output->data[i] = input->data[i];
  }
  return true;
}

velodyne_msgs__msg__VelodynePacket *
velodyne_msgs__msg__VelodynePacket__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodynePacket * msg = (velodyne_msgs__msg__VelodynePacket *)allocator.allocate(sizeof(velodyne_msgs__msg__VelodynePacket), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(velodyne_msgs__msg__VelodynePacket));
  bool success = velodyne_msgs__msg__VelodynePacket__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
velodyne_msgs__msg__VelodynePacket__destroy(velodyne_msgs__msg__VelodynePacket * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    velodyne_msgs__msg__VelodynePacket__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
velodyne_msgs__msg__VelodynePacket__Sequence__init(velodyne_msgs__msg__VelodynePacket__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodynePacket * data = NULL;

  if (size) {
    data = (velodyne_msgs__msg__VelodynePacket *)allocator.zero_allocate(size, sizeof(velodyne_msgs__msg__VelodynePacket), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = velodyne_msgs__msg__VelodynePacket__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        velodyne_msgs__msg__VelodynePacket__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
velodyne_msgs__msg__VelodynePacket__Sequence__fini(velodyne_msgs__msg__VelodynePacket__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      velodyne_msgs__msg__VelodynePacket__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

velodyne_msgs__msg__VelodynePacket__Sequence *
velodyne_msgs__msg__VelodynePacket__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodynePacket__Sequence * array = (velodyne_msgs__msg__VelodynePacket__Sequence *)allocator.allocate(sizeof(velodyne_msgs__msg__VelodynePacket__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = velodyne_msgs__msg__VelodynePacket__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
velodyne_msgs__msg__VelodynePacket__Sequence__destroy(velodyne_msgs__msg__VelodynePacket__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    velodyne_msgs__msg__VelodynePacket__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
velodyne_msgs__msg__VelodynePacket__Sequence__are_equal(const velodyne_msgs__msg__VelodynePacket__Sequence * lhs, const velodyne_msgs__msg__VelodynePacket__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!velodyne_msgs__msg__VelodynePacket__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
velodyne_msgs__msg__VelodynePacket__Sequence__copy(
  const velodyne_msgs__msg__VelodynePacket__Sequence * input,
  velodyne_msgs__msg__VelodynePacket__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(velodyne_msgs__msg__VelodynePacket);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    velodyne_msgs__msg__VelodynePacket * data =
      (velodyne_msgs__msg__VelodynePacket *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!velodyne_msgs__msg__VelodynePacket__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          velodyne_msgs__msg__VelodynePacket__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!velodyne_msgs__msg__VelodynePacket__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
