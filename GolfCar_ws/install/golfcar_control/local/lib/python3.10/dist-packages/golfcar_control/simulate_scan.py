#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that generates and publishes simulated LIDAR scan data based on a predefined map. 
The node reads the map parameters and data, simulates LIDAR scans, and publishes the scan data to the '/simulated_scan' topic. 
It also publishes a static transform between the map frame and the laser frame.

Published Topics:
- /simulated_scan (sensor_msgs/LaserScan): Publishes the simulated LIDAR scan data.

Subscribed Topics:
- None

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from nav_msgs.msg import OccupancyGrid
from sensor_msgs.msg import LaserScan
from geometry_msgs.msg import Quaternion, TransformStamped
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster
from PIL import Image
import numpy as np
import math
import os
import yaml

class SimulatedScanNode(Node):
    def __init__(self):
        super().__init__('simulated_scan_node')
        self.scan_pub = self.create_publisher(LaserScan, '/simulated_scan', 10)
        self.tf_broadcaster = StaticTransformBroadcaster(self)

        # Map-related parameters
        self.map_yaml_path = os.path.join(os.path.dirname(__file__), 'maps/golfCar_map.yaml')  # Update with the actual path to your map yaml file
        self.map_file_path, self.resolution, self.origin_x, self.origin_y, self.origin_yaw = self.load_map_params(self.map_yaml_path)

        self.get_logger().info(f'Loading map from {self.map_file_path}')
        self.map_data = self.load_map(self.map_file_path)

        if self.map_data is not None:
            self.get_logger().info('Map loaded successfully')
        else:
            self.get_logger().error('Failed to load the map')

        # Initial pose (replace with your desired initial pose)
        self.initial_pose_x = 0.0
        self.initial_pose_y = 0.0
        self.initial_pose_yaw = 0.0

        # Publish static transform between map and base_footprint
        self.publish_static_transform()

        # Timer to publish the scan at regular intervals
        self.timer = self.create_timer(1.0, self.generate_and_publish_scan)

    def load_map_params(self, yaml_file_path):
        try:
            with open(yaml_file_path, 'r') as file:
                map_params = yaml.safe_load(file)
                map_file_path = os.path.join(os.path.dirname(yaml_file_path), map_params['image'])
                resolution = map_params['resolution']
                origin = map_params['origin']
                origin_x, origin_y, origin_yaw = origin[0], origin[1], origin[2]
                return map_file_path, resolution, origin_x, origin_y, origin_yaw
        except FileNotFoundError:
            self.get_logger().error(f"YAML file not found: {yaml_file_path}")
            return None, None, None, None, None

    def load_map(self, map_file_path):
        try:
            map_img = Image.open(map_file_path).convert('L')  # Convert to grayscale
            map_array = np.array(map_img)
            map_array = np.flipud(map_array)  # Flip to align with the origin at bottom-left
            map_array = (map_array < 128).astype(int)  # Binarize: obstacles are 1, free space is 0
            return map_array
        except FileNotFoundError:
            self.get_logger().error(f"Map file not found: {map_file_path}")
            return None

    def publish_static_transform(self):
        t = TransformStamped()
        t.header.stamp = self.get_clock().now().to_msg()
        t.header.frame_id = 'map'
        t.child_frame_id = 'laser_frame'

        t.transform.translation.x = self.initial_pose_x
        t.transform.translation.y = self.initial_pose_y
        t.transform.translation.z = 0.0
        q = self.yaw_to_quaternion(self.initial_pose_yaw)
        t.transform.rotation = q

        self.tf_broadcaster.sendTransform(t)
        self.get_logger().info('Published static transform between map and base_footprint')

    def generate_and_publish_scan(self):
        if self.map_data is None:
            self.get_logger().error('Map data is not loaded.')
            return

        self.get_logger().info('Generating simulated scan')

        # Map parameters
        height, width = self.map_data.shape
        resolution = self.resolution

        # Laser parameters
        laser_range_min = 0.3  # Minimum range of the laser
        laser_range_max = 100.0  # Maximum range of the laser
        angle_min = -1.57  # Start angle of the scan [rad]
        angle_max = 1.57   # End angle of the scan [rad]
        samples = 2000  # Number of samples in the scan
        angle_increment = (angle_max - angle_min) / samples  # Angle increment [rad]

        # Initialize LaserScan message
        scan = LaserScan()
        scan.header.stamp = self.get_clock().now().to_msg()
        scan.header.frame_id = 'base_footprint'
        scan.angle_min = angle_min
        scan.angle_max = angle_max
        scan.angle_increment = angle_increment
        scan.range_min = laser_range_min
        scan.range_max = laser_range_max

        scan.ranges = [laser_range_max] * samples

        for i in range(samples):
            angle = angle_min + i * angle_increment
            for r in np.arange(laser_range_min, laser_range_max, resolution):
                x = self.initial_pose_x + r * math.cos(angle + self.initial_pose_yaw)
                y = self.initial_pose_y + r * math.sin(angle + self.initial_pose_yaw)

                map_x = int((x - self.origin_x) / resolution)
                map_y = int((y - self.origin_y) / resolution)

                if 0 <= map_x < width and 0 <= map_y < height:
                    if self.map_data[map_y, map_x] == 1:
                        scan.ranges[i] = r
                        break

        self.scan_pub.publish(scan)
        self.get_logger().info('Simulated scan published')

    def compute_distance_to_obstacle(self, x, y, resolution):
        return math.sqrt(x**2 + y**2)

    def yaw_to_quaternion(self, yaw):
        q = Quaternion()
        q.w = math.cos(yaw / 2.0)
        q.x = 0.0
        q.y = 0.0
        q.z = math.sin(yaw / 2.0)
        return q

def main(args=None):
    rclpy.init(args=args)
    node = SimulatedScanNode()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()

