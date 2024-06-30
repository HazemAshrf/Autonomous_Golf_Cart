cmake_minimum_required(VERSION 3.8)
project(golfcar_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Add the GeographicLib module path
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};/usr/share/cmake/geographiclib")

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(nav_msgs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(GeographicLib REQUIRED)

# Include directories
include_directories(
  include
  ${CMAKE_CURRENT_SOURCE_DIR}/include/golfcar_control
  ${GeographicLib_INCLUDE_DIRS}
)

# Add executables
add_executable(gps_to_odom_node src/gps_to_odom_node.cpp)
add_executable(orientation_calculator_node src/orientation_calculator_node.cpp)

# Link libraries
ament_target_dependencies(gps_to_odom_node rclcpp std_msgs sensor_msgs geometry_msgs nav_msgs tf2_ros yaml-cpp)
ament_target_dependencies(orientation_calculator_node rclcpp std_msgs sensor_msgs geometry_msgs nav_msgs tf2_ros yaml-cpp)
target_link_libraries(gps_to_odom_node ${GeographicLib_LIBRARIES})
target_link_libraries(orientation_calculator_node ${GeographicLib_LIBRARIES})

# Install targets
install(TARGETS
  gps_to_odom_node
  orientation_calculator_node
  DESTINATION lib/${PROJECT_NAME}
)

# Install other files (config, urdf, maps, worlds)
install(DIRECTORY config/
  DESTINATION share/${PROJECT_NAME}/config
)

install(DIRECTORY urdf/
  DESTINATION share/${PROJECT_NAME}/urdf
)

install(DIRECTORY maps/
  DESTINATION share/${PROJECT_NAME}/maps
)

install(DIRECTORY rviz/
  DESTINATION share/${PROJECT_NAME}/rviz
)

install(DIRECTORY worlds/
  DESTINATION share/${PROJECT_NAME}/worlds
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

# Python setup
ament_python_install_package(${PROJECT_NAME} PACKAGE_DIR scripts)

# Install Python scripts
install(PROGRAMS
  scripts/gps_to_odom.py
  scripts/odometry.py
  scripts/publish_goal.py
  scripts/publish_goal_exact_xy.py
  scripts/publish_goal_sim.py
  scripts/publish_goal_sim_exact_xy.py
  scripts/publish_gps_flask.py
  scripts/publish_gps_telegram.py
  scripts/publish_initial_pose.py
  scripts/simulate_scan.py
  scripts/send_commands.py
  scripts/stop_car_flask.py
  scripts/stop_car_telegram.py
  DESTINATION lib/${PROJECT_NAME}
)

ament_package()
