#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that converts GPS coordinates to odometry data. 
It subscribes to GPS data from a NavSatFix topic, converts the coordinates to 
local Cartesian coordinates, applies a transformation to odometry frame, and 
publishes the resulting odometry data.

Published Topics:
- /odometry/gps (nav_msgs/Odometry): Publishes the converted odometry data based on the GPS input.

Subscribed Topics:
- /fix (sensor_msgs/NavSatFix): Subscribes to GPS data for conversion to odometry.

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix
from nav_msgs.msg import Odometry
from pyproj import Proj
import math

class GpsToOdomNode(Node):
    def __init__(self):
        super().__init__('gps_to_odom_node')
        self.gps_sub_ = self.create_subscription(NavSatFix, '/gps/fix', self.gps_callback, 10)
        self.odom_pub_ = self.create_publisher(Odometry, '/odometry/gps', 10)
        self.angle_deg_ = 4.3  # The rotation angle between the map frame and the ENU frame
        self.lat_0 = 30.004695 # The latitude of the map frame
        self.lon_0 = 31.699729 # The longitude of the map frame
        self.local_cartesian_ = Proj(proj='aeqd', ellps='WGS84', datum='WGS84', lat_0=self.lat_0, lon_0=self.lon_0)
        self.get_logger().info(f'Node initialized. Initial position set to (lat, lon):({self.lat_0}, {self.lon_0})')

    def transform_enu_to_odom(self, enu_x, enu_y):
        """
        Transform ENU coordinates to odometry frame.

        Parameters:
            enu_x: ENU x-coordinate.
            enu_y: ENU y-coordinate.

        Returns:
            (odom_x, odom_y): Transformed odometry coordinates.
        """
        angle_rad = self.angle_deg_ * math.pi / 180.0
        cos_theta = math.cos(angle_rad)
        sin_theta = math.sin(angle_rad)
        odom_x = enu_x * cos_theta - enu_y * sin_theta
        odom_y = enu_x * sin_theta + enu_y * cos_theta
        return odom_x, odom_y

    def gps_callback(self, msg):
        """
        Callback function to process incoming GPS data and publish odometry data.

        Parameters:
            msg: The incoming NavSatFix message containing GPS data.
        """
        # Check the GPS fix status
        if msg.status.status == -1:  # Check for STATUS_NO_FIX
            self.get_logger().warn('No GPS fix available.')
            return

        enu_x, enu_y = self.local_cartesian_(msg.longitude, msg.latitude)
        odom_x, odom_y = self.transform_enu_to_odom(enu_x, enu_y)

        odom_msg = Odometry()
        odom_msg.header.stamp = msg.header.stamp
        odom_msg.header.frame_id = 'odom'
        odom_msg.child_frame_id = 'base_footprint'

        odom_msg.pose.pose.position.x = odom_x
        odom_msg.pose.pose.position.y = odom_y
        odom_msg.pose.pose.position.z = 0.0
        odom_msg.pose.pose.orientation.x = 0.0
        odom_msg.pose.pose.orientation.y = 0.0
        odom_msg.pose.pose.orientation.z = 0.0
        odom_msg.pose.pose.orientation.w = 1.0

        odom_msg.pose.covariance = [0.0] * 36

        self.odom_pub_.publish(odom_msg)
        self.get_logger().info(f'Published odometry: x={odom_x}, y={odom_y}')

def main(args=None):
    rclpy.init(args=args)
    node = GpsToOdomNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

