// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from velodyne_msgs:msg/VelodyneScan.idl
// generated code does not contain a copyright notice

#ifndef VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_HPP_
#define VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'packets'
#include "velodyne_msgs/msg/detail/velodyne_packet__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__velodyne_msgs__msg__VelodyneScan __attribute__((deprecated))
#else
# define DEPRECATED__velodyne_msgs__msg__VelodyneScan __declspec(deprecated)
#endif

namespace velodyne_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct VelodyneScan_
{
  using Type = VelodyneScan_<ContainerAllocator>;

  explicit VelodyneScan_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit VelodyneScan_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _packets_type =
    std::vector<velodyne_msgs::msg::VelodynePacket_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<velodyne_msgs::msg::VelodynePacket_<ContainerAllocator>>>;
  _packets_type packets;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__packets(
    const std::vector<velodyne_msgs::msg::VelodynePacket_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<velodyne_msgs::msg::VelodynePacket_<ContainerAllocator>>> & _arg)
  {
    this->packets = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> *;
  using ConstRawPtr =
    const velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__velodyne_msgs__msg__VelodyneScan
    std::shared_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__velodyne_msgs__msg__VelodyneScan
    std::shared_ptr<velodyne_msgs::msg::VelodyneScan_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const VelodyneScan_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->packets != other.packets) {
      return false;
    }
    return true;
  }
  bool operator!=(const VelodyneScan_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct VelodyneScan_

// alias to use template instance with default allocator
using VelodyneScan =
  velodyne_msgs::msg::VelodyneScan_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace velodyne_msgs

#endif  // VELODYNE_MSGS__MSG__DETAIL__VELODYNE_SCAN__STRUCT_HPP_
