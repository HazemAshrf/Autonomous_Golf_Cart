// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from velodyne_msgs:msg/VelodyneScan.idl
// generated code does not contain a copyright notice
#include "velodyne_msgs/msg/detail/velodyne_scan__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `packets`
#include "velodyne_msgs/msg/detail/velodyne_packet__functions.h"

bool
velodyne_msgs__msg__VelodyneScan__init(velodyne_msgs__msg__VelodyneScan * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    velodyne_msgs__msg__VelodyneScan__fini(msg);
    return false;
  }
  // packets
  if (!velodyne_msgs__msg__VelodynePacket__Sequence__init(&msg->packets, 0)) {
    velodyne_msgs__msg__VelodyneScan__fini(msg);
    return false;
  }
  return true;
}

void
velodyne_msgs__msg__VelodyneScan__fini(velodyne_msgs__msg__VelodyneScan * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // packets
  velodyne_msgs__msg__VelodynePacket__Sequence__fini(&msg->packets);
}

bool
velodyne_msgs__msg__VelodyneScan__are_equal(const velodyne_msgs__msg__VelodyneScan * lhs, const velodyne_msgs__msg__VelodyneScan * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // packets
  if (!velodyne_msgs__msg__VelodynePacket__Sequence__are_equal(
      &(lhs->packets), &(rhs->packets)))
  {
    return false;
  }
  return true;
}

bool
velodyne_msgs__msg__VelodyneScan__copy(
  const velodyne_msgs__msg__VelodyneScan * input,
  velodyne_msgs__msg__VelodyneScan * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // packets
  if (!velodyne_msgs__msg__VelodynePacket__Sequence__copy(
      &(input->packets), &(output->packets)))
  {
    return false;
  }
  return true;
}

velodyne_msgs__msg__VelodyneScan *
velodyne_msgs__msg__VelodyneScan__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodyneScan * msg = (velodyne_msgs__msg__VelodyneScan *)allocator.allocate(sizeof(velodyne_msgs__msg__VelodyneScan), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(velodyne_msgs__msg__VelodyneScan));
  bool success = velodyne_msgs__msg__VelodyneScan__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
velodyne_msgs__msg__VelodyneScan__destroy(velodyne_msgs__msg__VelodyneScan * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    velodyne_msgs__msg__VelodyneScan__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
velodyne_msgs__msg__VelodyneScan__Sequence__init(velodyne_msgs__msg__VelodyneScan__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodyneScan * data = NULL;

  if (size) {
    data = (velodyne_msgs__msg__VelodyneScan *)allocator.zero_allocate(size, sizeof(velodyne_msgs__msg__VelodyneScan), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = velodyne_msgs__msg__VelodyneScan__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        velodyne_msgs__msg__VelodyneScan__fini(&data[i - 1]);
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
velodyne_msgs__msg__VelodyneScan__Sequence__fini(velodyne_msgs__msg__VelodyneScan__Sequence * array)
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
      velodyne_msgs__msg__VelodyneScan__fini(&array->data[i]);
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

velodyne_msgs__msg__VelodyneScan__Sequence *
velodyne_msgs__msg__VelodyneScan__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  velodyne_msgs__msg__VelodyneScan__Sequence * array = (velodyne_msgs__msg__VelodyneScan__Sequence *)allocator.allocate(sizeof(velodyne_msgs__msg__VelodyneScan__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = velodyne_msgs__msg__VelodyneScan__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
velodyne_msgs__msg__VelodyneScan__Sequence__destroy(velodyne_msgs__msg__VelodyneScan__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    velodyne_msgs__msg__VelodyneScan__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
velodyne_msgs__msg__VelodyneScan__Sequence__are_equal(const velodyne_msgs__msg__VelodyneScan__Sequence * lhs, const velodyne_msgs__msg__VelodyneScan__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!velodyne_msgs__msg__VelodyneScan__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
velodyne_msgs__msg__VelodyneScan__Sequence__copy(
  const velodyne_msgs__msg__VelodyneScan__Sequence * input,
  velodyne_msgs__msg__VelodyneScan__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(velodyne_msgs__msg__VelodyneScan);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    velodyne_msgs__msg__VelodyneScan * data =
      (velodyne_msgs__msg__VelodyneScan *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!velodyne_msgs__msg__VelodyneScan__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          velodyne_msgs__msg__VelodyneScan__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!velodyne_msgs__msg__VelodyneScan__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
