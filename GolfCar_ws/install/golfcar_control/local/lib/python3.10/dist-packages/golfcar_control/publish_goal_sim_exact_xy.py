#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes a goal position in gazebo based on a target GPS location
defined as "self.target_latitude" and "self.target_longitude". It converts the coordinates to a local Cartesian frame, 
transforms them to the map frame, and publishes the resulting goal position as PoseStamped messages.

Notes:
- This code can only calculate the (x, y) for the goal location,
  but the heading (yaw) needs to be set manually using the variable "self.goal_heading".
- The GPS location of the map frame is defined in the world file.

Published Topics:
- /goal_pose (geometry_msgs/PoseStamped): Publishes the goal position and orientation.

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseStamped
from pyproj import Proj
import math

class GoalPublisher(Node):
    def __init__(self):
        super().__init__('goal_publisher')
        self.goal_pub_ = self.create_publisher(PoseStamped, 'goal_pose', 10)
        
        self.angle_deg_ = 180  # The rotation angle between the map frame and the ENU frame
        self.lat_0 = 30.0695   # The latitude of the map frame
        self.lon_0 = 31.5122   # The longitude of the map frame
        self.local_cartesian_ = Proj(proj='aeqd', ellps='WGS84', datum='WGS84', lat_0=self.lat_0, lon_0=self.lon_0)

        # Define the target GPS coordinates (latitude and longitude)
        self.target_latitude = 30.06918262810449
        self.target_longitude = 31.51208030027126

        # Define the heading for the goal location (in degrees)
        self.goal_heading = 90  # Example heading, replace with actual value

    def transform_enu_to_map(self, enu_x, enu_y):
        """
        Transform ENU coordinates to the map frame.

        Parameters:
            enu_x: ENU x-coordinate.
            enu_y: ENU y-coordinate.

        Returns:
            (map_x, map_y): Transformed map coordinates.
        """
        angle_rad = self.angle_deg_ * math.pi / 180.0
        cos_theta = math.cos(angle_rad)
        sin_theta = math.sin(angle_rad)
        map_x = enu_x * cos_theta - enu_y * sin_theta
        map_y = enu_x * sin_theta + enu_y * cos_theta
        return map_x, map_y

    def heading_to_quaternion(self, heading):
        # Convert heading in degrees to radians
        heading_rad = math.radians(heading)
        # Calculate quaternion for heading
        qx = 0.0
        qy = 0.0
        qz = math.sin(heading_rad / 2.0)
        qw = math.cos(heading_rad / 2.0)
        return [qx, qy, qz, qw]

    def set_goal_from_location(self, latitude, longitude):
        # Convert target GPS coordinates to ENU
        enu_x, enu_y = self.local_cartesian_(longitude, latitude)
        map_x, map_y = self.transform_enu_to_map(enu_x, enu_y)

        # Convert heading to quaternion
        goal_orientation = self.heading_to_quaternion(self.goal_heading)

        # Create and publish goal PoseStamped message
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = map_x
        goal_pose.pose.position.y = map_y
        goal_pose.pose.position.z = 0.0
        goal_pose.pose.orientation.x = goal_orientation[0]
        goal_pose.pose.orientation.y = goal_orientation[1]
        goal_pose.pose.orientation.z = goal_orientation[2]
        goal_pose.pose.orientation.w = goal_orientation[3]

        self.goal_pub_.publish(goal_pose)
        self.get_logger().info(f'Published goal coordinates: x={map_x}, y={map_y}, heading={self.goal_heading}')


def main(args=None):
    rclpy.init(args=args)
    node = GoalPublisher()
    
    node.set_goal_from_location(node.target_latitude, node.target_longitude)
    
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

