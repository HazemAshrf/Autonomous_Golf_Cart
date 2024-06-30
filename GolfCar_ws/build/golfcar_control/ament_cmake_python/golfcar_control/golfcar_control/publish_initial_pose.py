#!/usr/bin/env python3

"""
Description:
This script implements an integrated ROS2 node that combines GPS, magnetometer, and LIDAR data for estimating the initial position. 
The node subscribes to GPS, magnetometer, and LIDAR topics, processes the data to set the initial position, 
simulates LIDAR scans based on the map, and performs ICP (Iterative Closest Point) registration to refine the pose estimate.

Published Topics:
- /simulated_scan (sensor_msgs/LaserScan): Publishes the simulated LIDAR scan data.
- /initialpose (geometry_msgs/PoseWithCovarianceStamped): Publishes the corrected initial pose based on ICP registration.

Subscribed Topics:
- /fix (sensor_msgs/NavSatFix): Subscribes to GPS data to set the initial position.
- /scan (sensor_msgs/LaserScan): Subscribes to actual LIDAR scan data.
- /magnetometer/direction (std_msgs/Float32): Subscribes to magnetometer data to set the initial orientation.

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix, LaserScan
from std_msgs.msg import Float32  # Import Float32 for the magnetometer data
from geometry_msgs.msg import PoseWithCovarianceStamped, Quaternion, TransformStamped
from pyproj import Proj
from PIL import Image
import numpy as np
import math
import os
import yaml
import open3d as o3d
import matplotlib.pyplot as plt
import time

class InitialPositionPublisher(Node):
    def __init__(self):
        super().__init__('initial_position_publisher')

        # Subscriptions and Publishers
        self.gps_sub = self.create_subscription(NavSatFix, '/fix', self.gps_callback, 10)
        self.scan_pub = self.create_publisher(LaserScan, '/simulated_scan', 10)
        self.pose_pub = self.create_publisher(PoseWithCovarianceStamped, '/initialpose', 10)

        # Magnetometer subscription
        self.mag_sub = self.create_subscription(Float32, 'magnetometer/direction', self.mag_callback, 10)

        # GPS and map parameters
        self.initial_position_set = False
        self.map_lat = 30.004695  # The latitude of the map frame
        self.map_lon = 31.699729  # The longitude of the map frame
        self.angle_deg = 4.3      # The rotation angle between the map frame and the ENU frame
        self.mag_offset = 6.5     # The Magnetometer direction when the heading (yaw) = 0.0
        self.orientation = None   # Will be set by magnetometer data
        self.orientation_received = False  # Flag to ensure orientation is set only once

        self.local_cartesian = Proj(proj='aeqd', ellps='WGS84', datum='WGS84', lat_0=self.map_lat, lon_0=self.map_lon)

        # Map loading
        self.map_yaml_path = os.path.join('/home/hazem/GolfCar_ws/src/maps/GIU_map.yaml')
        self.map_file_path, self.resolution, self.origin_x, self.origin_y, self.origin_yaw = self.load_map_params(self.map_yaml_path)
        self.map_data = self.load_map(self.map_file_path)
        if self.map_data is not None:
            self.get_logger().info('Map loaded successfully')
        else:
            self.get_logger().error('Failed to load the map')

        self.initial_pose_x = None
        self.initial_pose_y = None
        self.initial_pose_yaw = None

        self.actual_scan = None
        self.simulated_scan_data = None  # Store scan data to be published to /simulated_scan

        self.get_logger().info('Node initialized. Waiting for first GPS fix to set initial position.')

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

    def transform_enu_to_map(self, enu_x, enu_y):
        """
        Transform ENU coordinates to map coordinates.

        Parameters:
            enu_x: ENU x-coordinate.
            enu_y: ENU y-coordinate.

        Returns:
            (map_x, map_y): Transformed map coordinates.
        """
        angle_rad = self.angle_deg * math.pi / 180.0
        cos_theta = math.cos(angle_rad)
        sin_theta = math.sin(angle_rad)
        map_x = enu_x * cos_theta - enu_y * sin_theta
        map_y = enu_x * sin_theta + enu_y * cos_theta
        return map_x, map_y

    def mag_callback(self, msg):
        if not self.orientation_received:
            self.orientation = msg.data - self.mag_offset
            self.orientation = math.radians(msg.data)  # Convert to radians
            self.orientation_received = True
            self.get_logger().info(f'Orientation set from magnetometer data: {self.orientation} radians')

    def gps_callback(self, msg):
        if self.initial_position_set:
            return

        if msg.status.status == -1:  # Check for STATUS_NO_FIX
            self.get_logger().warn('No GPS fix available.')
            return

        # Convert GPS to local Cartesian coordinates relative to map frame origin
        enu_x, enu_y = self.local_cartesian(msg.longitude, msg.latitude)
        map_x, map_y = self.transform_enu_to_map(enu_x, enu_y)

        # Set initial pose
        if self.orientation is None:
            self.get_logger().warn('Waiting for orientation data from magnetometer.')
            return

        self.initial_pose_x = map_x
        self.initial_pose_y = map_y
        self.initial_pose_yaw = -self.orientation

        self.initial_position_set = True
        self.get_logger().info(f'Initial position set to (x, y, yaw): ({map_x}, {map_y}, {self.orientation})')

        # Now that the initial position is set, subscribe to scan topic
        self.scan_sub = self.create_subscription(LaserScan, '/scan', self.scan_callback, 10)

    def simulate_and_publish_scan(self):
        """
        Simulate a LIDAR scan based on the map data and publish it.
        """
        if self.initial_pose_x is None or self.initial_pose_y is None:
            self.get_logger().error('Simulated scan or initial pose is not available.')
            return

        laser_range_min = 0.45  # Minimum range of the laser
        laser_range_max = 100.0  # Maximum range of the laser
        angle_min = -1.5708  # Minimum angle of the scan (-90 degrees)
        angle_max = 1.5708  # Maximum angle of the scan (90 degrees)
        #samples = 2000  # Number of samples in the scan
        angle_increment = 0.0020940001122653484  # Angle increment per sample
        samples = int((angle_max - angle_min) /angle_increment)
        print(samples)
        

        scan = LaserScan()
        scan.header.stamp = self.get_clock().now().to_msg()
        scan.header.frame_id = 'laser_frame'
        scan.angle_min = angle_min
        scan.angle_max = angle_max
        scan.angle_increment = angle_increment
        scan.range_min = laser_range_min
        scan.range_max = laser_range_max

        scan.ranges = [laser_range_max] * samples  # Initialize all ranges to the maximum

        height, width = self.map_data.shape
        resolution = self.resolution
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

        self.simulated_scan_data = scan  # Store the scan data
        self.scan_pub.publish(scan)
        self.get_logger().info('Simulated scan published')

    def scan_callback(self, msg):
        if not self.initial_position_set:
            self.get_logger().warn('Initial position not set yet.')
            return

        self.get_logger().info('Received actual scan')
        self.actual_scan = self.convert_scan_to_points(msg)
        self.get_logger().info('Actual scan converted to points')
        self.simulate_and_publish_scan()
        if self.simulated_scan_data is not None:
            self.simulated_scan_points = self.convert_scan_to_points(self.simulated_scan_data)
            self.perform_icp_and_publish(self.actual_scan, self.simulated_scan_points)
            raise SystemExit
        else:
            self.get_logger().warn('Simulated scan data not available for conversion')

    def convert_scan_to_points(self, scan):
        """
        Convert LIDAR scan data (ranges and angles) to a list of (x, y) points.

        Parameters:
            scan: The LaserScan message to be converted.

        Returns:
            points: List of (x, y) coordinates.
        """
        points = []
        angle = scan.angle_min
        for r in scan.ranges:
            if r < scan.range_max:
                x = r * math.cos(angle)
                y = r * math.sin(angle)
                points.append((x, y))
            angle += scan.angle_increment
        self.get_logger().info(f'Converted scan to points: {len(points)} points')
        return points

    def perform_icp_and_publish(self, actual_points, simulated_points):
        """
        Perform ICP registration and publish the corrected initial pose.

        Parameters:
            actual_points: List of actual LIDAR scan points.
            simulated_points: List of simulated LIDAR scan points.
        """
        actual_points = np.array(actual_points)
        simulated_points = np.array(simulated_points)

        actual_points_3d = np.hstack((actual_points, np.zeros((actual_points.shape[0], 1))))
        simulated_points_3d = np.hstack((simulated_points, np.zeros((simulated_points.shape[0], 1))))

        actual_pcd = o3d.geometry.PointCloud()
        actual_pcd.points = o3d.utility.Vector3dVector(actual_points_3d)
        actual_colors = np.array([[1, 0, 0] for _ in range(actual_points_3d.shape[0])])  # Red color
        actual_pcd.colors = o3d.utility.Vector3dVector(actual_colors)

        simulated_pcd = o3d.geometry.PointCloud()
        simulated_pcd.points = o3d.utility.Vector3dVector(simulated_points_3d)
        simulated_colors = np.array([[0, 0, 1] for _ in range(simulated_points_3d.shape[0])])  # Blue color
        simulated_pcd.colors = o3d.utility.Vector3dVector(simulated_colors)

        threshold = 30.0  # Maximum correspondence points-pair distance
        trans_init = np.eye(4)
        
        self.draw_registration_result(simulated_pcd, actual_pcd, trans_init, title="Before Registration")

        reg_p2p = o3d.pipelines.registration.registration_icp(
            simulated_pcd, actual_pcd, threshold, trans_init,
            o3d.pipelines.registration.TransformationEstimationPointToPoint(),
            o3d.pipelines.registration.ICPConvergenceCriteria(max_iteration=2000))

        transform1 = reg_p2p.transformation
        
        self.draw_registration_result(simulated_pcd, actual_pcd, transform1, title="After First Registration")

        # Apply first transformation to the simulated points
        transformed_points_3d = np.dot(simulated_points_3d, transform1[:3, :3].T) + transform1[:3, 3]
        transformed_points = transformed_points_3d[:, :2]

        transformed_pcd = o3d.geometry.PointCloud()
        transformed_pcd.points = o3d.utility.Vector3dVector(transformed_points_3d)
        transformed_pcd.colors = o3d.utility.Vector3dVector(simulated_colors)  # Keep simulated color for transformed points

        threshold = 5.0
        reg_p2p2 = o3d.pipelines.registration.registration_icp(
            transformed_pcd, actual_pcd, threshold, trans_init,
            o3d.pipelines.registration.TransformationEstimationPointToPoint(),
            o3d.pipelines.registration.ICPConvergenceCriteria(max_iteration=2000))

        transform2 = reg_p2p2.transformation
        
        self.draw_registration_result(transformed_pcd, actual_pcd, transform2, title="After Second Registration")
        
        # Combine the two transformations
        combined_transform = np.dot(transform2, transform1)

        # Extract x, y, and yaw from the combined transformation matrix
        x = self.initial_pose_x - combined_transform[0, 3]
        y = self.initial_pose_y - combined_transform[1, 3]
        yaw = self.initial_pose_yaw - np.arctan2(combined_transform[1, 0], combined_transform[0, 0])

        self.get_logger().info(f'Combined Transform: x = {x}, y = {y}, yaw = {yaw}')

        # Publish the corrected initial pose
        self.publish_corrected_initial_pose(x, y, yaw)
        
    def draw_registration_result(self, source, target, transformation, title=""):
        """
        Draw the registration result using Open3D.

        Parameters:
            source: Source point cloud.
            target: Target point cloud.
            transformation: Transformation matrix.
            title: Title of the visualization window.
        """
        source_temp = source.transform(transformation)
        o3d.visualization.draw_geometries([source_temp, target], window_name=title)

    def publish_corrected_initial_pose(self, x, y, yaw):
        corrected_pose = PoseWithCovarianceStamped()
        corrected_pose.header.stamp = self.get_clock().now().to_msg()
        corrected_pose.header.frame_id = 'map'

        corrected_pose.pose.pose.position.x = x
        corrected_pose.pose.pose.position.y = y
        corrected_pose.pose.pose.position.z = 0.0

        q = self.yaw_to_quaternion(yaw)
        corrected_pose.pose.pose.orientation = q

        corrected_pose.pose.covariance = [0.0] * 36

        self.pose_pub.publish(corrected_pose)
        self.get_logger().info('Published corrected initial pose.')

    def yaw_to_quaternion(self, yaw):
        q = Quaternion()
        q.w = math.cos(yaw / 2.0)
        q.x = 0.0
        q.y = 0.0
        q.z = math.sin(yaw / 2.0)
        return q

def main(args=None):
    rclpy.init(args=args)
    node = InitialPositionPublisher()
    try:
        rclpy.spin(node)
    except SystemExit:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()


