#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes a goal position in gazebo based on a target GPS location
defined as "self.target_latitude" and "self.target_longitude". It compares the goal GPS location with predefined 
locations and determines the closest target to send its corresponding (x, y, yaw).

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

        # Define the target GPS coordinates (latitude and longitude) to compare against
        self.target_latitude = 30.06966
        self.target_longitude = 31.511

        # Define the target locations with (x, y, heading) and corresponding GPS coordinates
        self.targets = [
            {"lat": 30.069184155830584, "lon": 31.512087901309823, "x": 11.284577401778355, "y": 37.00858574571807, "heading": 180},
            {"lat": 30.069574254653986, "lon": 31.512584617082283, "x": -36.60190410824849, "y": -6.226476068994273, "heading": 270},
            {"lat": 30.06966901187551, "lon": 31.511999923675074, "x": 19.768817973354185, "y": -16.750961744332443, "heading": 90}
        ]

    def heading_to_quaternion(self, heading):
        # Convert heading in degrees to radians
        heading_rad = math.radians(heading)
        # Calculate quaternion for heading
        qx = 0.0
        qy = 0.0
        qz = math.sin(heading_rad / 2.0)
        qw = math.cos(heading_rad / 2.0)
        return [qx, qy, qz, qw]

    def calculate_distance(self, lat1, lon1, lat2, lon2):
        """
        Calculate the distance between two GPS coordinates using the Haversine formula.

        Parameters:
            lat1: Latitude of the first point.
            lon1: Longitude of the first point.
            lat2: Latitude of the second point.
            lon2: Longitude of the second point.

        Returns:
            Distance in meters.
        """
        R = 6371000  # radius of Earth in meters
        phi_1 = math.radians(lat1)
        phi_2 = math.radians(lat2)
        delta_phi = math.radians(lat2 - lat1)
        delta_lambda = math.radians(lon2 - lon1)
        a = math.sin(delta_phi / 2.0) ** 2 + math.cos(phi_1) * math.cos(phi_2) * math.sin(delta_lambda / 2.0) ** 2
        c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
        meters = R * c  # output distance in meters
        return meters

    def set_goal_from_location(self, latitude, longitude):
        """
        Set the goal position based on the closest predefined target location to the given GPS coordinates.

        Parameters:
            latitude: Latitude of the current location.
            longitude: Longitude of the current location.
        """
        # Find the closest target
        closest_target = None
        min_distance = float('inf')
        for target in self.targets:
            distance = self.calculate_distance(latitude, longitude, target['lat'], target['lon'])
            if distance < min_distance:
                min_distance = distance
                closest_target = target

        if not closest_target:
            self.get_logger().warn('No closest target found.')
            return

        # Convert heading to quaternion
        goal_orientation = self.heading_to_quaternion(closest_target['heading'])

        # Create and publish goal PoseStamped message
        goal_pose = PoseStamped()
        goal_pose.header.frame_id = 'map'
        goal_pose.header.stamp = self.get_clock().now().to_msg()
        goal_pose.pose.position.x = closest_target['x']
        goal_pose.pose.position.y = closest_target['y']
        goal_pose.pose.position.z = 0.0
        goal_pose.pose.orientation.x = goal_orientation[0]
        goal_pose.pose.orientation.y = goal_orientation[1]
        goal_pose.pose.orientation.z = goal_orientation[2]
        goal_pose.pose.orientation.w = goal_orientation[3]

        self.goal_pub_.publish(goal_pose)
        self.get_logger().info(f'Published goal coordinates: x={closest_target["x"]}, y={closest_target["y"]}, heading={closest_target["heading"]}')

def main(args=None):
    rclpy.init(args=args)
    node = GoalPublisher()
    
    node.set_goal_from_location(node.target_latitude, node.target_longitude)
    
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

