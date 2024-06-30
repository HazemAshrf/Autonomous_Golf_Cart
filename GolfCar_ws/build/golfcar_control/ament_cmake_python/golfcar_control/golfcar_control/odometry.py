#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node for odometry using proximity sensors,
that works as a custom wheel encoder system, and IMU data from a serial communication interface.

The node processes linear and angular velocity data to estimate the position and orientation
of a point in 2D space. The IMU and the estimated odometry data are published.
The node also handles incoming velocity commands to adjust the movement accordingly.

Published Topics:
- /imu/data_raw (sensor_msgs/Imu): Publishes raw IMU data including acceleration and angular velocity.
- /odom (nav_msgs/Odometry): Publishes the calculated odometry information of the point.
- /magnetometer/direction (std_msgs/Float32): Publishes the direction from the magnetometer.

Published Transforms:
- odom -> base_footprint: Broadcasts the transform between the odometry frame and the base footprint of the robot.

Subscribed Topics:
- /cmd_vel (geometry_msgs/Twist): Subscribes to velocity commands to adjust the direction of movement of the point.

Author: Hazem Ashraf
"""

import math
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Imu
from geometry_msgs.msg import TransformStamped, Twist
from nav_msgs.msg import Odometry
from std_msgs.msg import Float32
import serial
import time
import tf2_ros

serial_comm = serial.Serial()

class Point:
    def __init__(self, initial_x=0, initial_y=0):
        """
        Initialize the Point object with given initial coordinates and heading.

        Parameters:
            initial_x: Initial x-coordinate of the point (default is 0).
            initial_y: Initial y-coordinate of the point (default is 0).
        """
        self.x = initial_x
        self.y = initial_y
        self.heading = 0  # Initialize heading angle

    def move(self, linear_velocity, angular_velocity, dt):
        """
        Move the point based on measured linear and angular velocities.

        Parameters:
            linear_velocity: Linear velocity of the point (m/s).
            angular_velocity: Angular velocity of the point (rad/s).
            dt: Time step for integration (s).
        """
        if linear_velocity != 0:
            self.heading += angular_velocity * dt
        
        dx = linear_velocity * math.cos(self.heading) * dt
        dy = linear_velocity * math.sin(self.heading) * dt
        
        self.x += dx
        self.y += dy

class OdometryNode(Node):
    
    def __init__(self):
        """
        Initialize the OdometryNode, set up serial communication, and define 
        publishers and subscribers.
        """
        super().__init__("serial_comm")
        self.point = Point()  # Initialize the point
        
        serial_port = "/dev/ttyUSB0"
        baud_rate = 115200
        self.connect_serial_port(serial_port=serial_port, baud_rate=baud_rate)
        time.sleep(0.5)
        
        self.imu_publisher_ = self.create_publisher(Imu, "imu/data_raw", 10)
        self.odometry_publisher_ = self.create_publisher(Odometry, 'odom', 10)
        self.tf_broadcaster = tf2_ros.TransformBroadcaster(self)
        self.magnetometer_publisher_ = self.create_publisher(Float32, 'magnetometer/direction', 10)
        self.mag_published = False  # Flag to ensure publishing only once
        
        # Subscriber for /cmd_vel topic
        self.cmd_vel_subscription = self.create_subscription(
            Twist,
            '/cmd_vel',
            self.cmd_vel_callback,
            1
        )

        self.get_logger().info("Communication has started.")
        self.timer_ = self.create_timer(0.005, self.read_data)
        
        # Initialize variable to track if linear velocity should be negated
        self.negate_linear_velocity = False

    def connect_serial_port(self, serial_port, baud_rate):
        """
        Connect to the serial port with the specified parameters.

        Parameters:
            serial_port: The serial port to connect to.
            baud_rate: The baud rate for the serial communication.
        """
        serial_comm.port = serial_port
        serial_comm.baudrate = baud_rate
        serial_comm.timeout = 1
        serial_comm.open()

    def cmd_vel_callback(self, msg):
        """
        Callback function to handle /cmd_vel messages.
        """
        # Check if linear.x is negative and set the flag accordingly
        if msg.linear.x < 0:
            self.negate_linear_velocity = True
        elif msg.linear.x > 0:
            self.negate_linear_velocity = False

    def read_data(self):
        """
        Read data from the serial interface, process it, and publish IMU, odometry,
        and magnetometer data.
        """
        try:
            imu_msg = Imu()
            imu_msg.header.stamp = self.get_clock().now().to_msg()
            imu_msg.header.frame_id = "IMU"
            odometry_msg = Odometry()
            odometry_msg.header.stamp = self.get_clock().now().to_msg()
            data = serial_comm.readline().decode("utf-8").rstrip("\n").rstrip("\r").split(',')
            
            # Reading acceleration
            imu_msg.linear_acceleration.x = float(data[0])
            imu_msg.linear_acceleration.y = float(data[1])
            imu_msg.linear_acceleration.z = float(data[2])
            
            # Reading angular velocity
            imu_msg.angular_velocity.x = float(data[3])
            imu_msg.angular_velocity.y = float(data[4])
            imu_msg.angular_velocity.z = float(data[5])
            
            angular_velocity = float(data[5])
            linear_velocity = float(data[7])
            dt = float(data[8])

            # Negate linear_velocity if the flag is set
            if self.negate_linear_velocity:
                linear_velocity = -linear_velocity

            # Reading magnetometer direction
            magnetometer_direction = float(data[6])

            self.imu_publisher_.publish(imu_msg)

            # Move the point based on measured velocities
            self.point.move(linear_velocity, angular_velocity, dt)
            
            # Publish odometry data
            odometry_msg.header.frame_id = 'odom'
            odometry_msg.child_frame_id = 'base_footprint'
            odometry_msg.pose.pose.position.x = self.point.x
            odometry_msg.pose.pose.position.y = self.point.y
            odometry_msg.pose.pose.orientation.z = math.sin(self.point.heading / 2)
            odometry_msg.pose.pose.orientation.w = math.cos(self.point.heading / 2)
            odometry_msg.twist.twist.linear.x = linear_velocity
            odometry_msg.twist.twist.angular.z = angular_velocity
            self.odometry_publisher_.publish(odometry_msg)

            # Publish magnetometer direction once
            if not self.mag_published:
                mag_msg = Float32()
                mag_msg.data = magnetometer_direction
                self.magnetometer_publisher_.publish(mag_msg)
                self.mag_published = True  # Ensure it is published only once

            print(odometry_msg.pose.pose.position.x, odometry_msg.pose.pose.position.y, self.point.heading)
            
            # Publish the transform
            odom_transform = TransformStamped()
            odom_transform.header.stamp = self.get_clock().now().to_msg()
            odom_transform.header.frame_id = 'odom'
            odom_transform.child_frame_id = 'base_footprint'
            odom_transform.transform.translation.x = self.point.x
            odom_transform.transform.translation.y = self.point.y
            odom_transform.transform.rotation.z = math.sin(self.point.heading / 2)
            odom_transform.transform.rotation.w = math.cos(self.point.heading / 2)
            self.tf_broadcaster.sendTransform(odom_transform)            

        except Exception as e:
            self.get_logger().warn("Communication error.")
            self.get_logger().warn(repr(e))

def main():
    rclpy.init()
    node = OdometryNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

