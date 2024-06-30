#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that converts Twist messages to linear velocity and steering angle commands 
for the Arduino-controlled car. The node subscribes to the '/cmd_vel' topic for movement commands and the 
'/stop_car' topic to stop the vehicle if the user wants to. It calculates the required steering angle and linear velocity and sends 
the commands to the Arduino via a serial connection.

Published Topics:
- None

Subscribed Topics:
- /cmd_vel (geometry_msgs/Twist): Subscribes to velocity commands to control the vehicle.
- /stop_car (std_msgs/Int32): Subscribes to stop commands to stop the vehicle and shut down the node.

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import Int32
import math
import serial
import time
import struct

class TwistConverterNode(Node):
    def __init__(self):
        """
        Initialize the TwistConverterNode, set up subscriptions to 'cmd_vel' and 'stop_car' topics,
        and initialize the serial connection to the Arduino.
        """
        super().__init__('twist_converter_node')

        # Subscribe to the cmd_vel topic
        self.subscription = self.create_subscription(
            Twist,
            'cmd_vel',
            self.twist_callback,
            1)
        
        # Subscribe to the stop_car topic
        self.stop_subscription = self.create_subscription(
            Int32,
            'stop_car',
            self.stop_car_callback,
            1)

        # Initialize wheel parameters
        self.wheel_separation = 1.2
        self.wheel_base = 2.81
        self.steering_angle = 0.0

        # Initialize serial connection to Arduino
        self.arduino = serial.Serial(port='/dev/ttyACM1', baudrate=115200, timeout=.1)

    def twist_callback(self, msg):
        # Convert the twist message to linear velocity and steering angle
        linear_velocity, steering_angle = self.convert_twist_to_velocity_and_steering(msg)

        # Send the data to Arduino
        self.send_data_to_arduino(steering_angle, linear_velocity)

    def stop_car_callback(self, msg):
        if msg.data == 1:
            # Prepare the stop command bytes
            command_bytes = struct.pack('<fi', 0.0, 0)
            
            # Send the stop command to Arduino
            print(f"Received stop signal. Sending stop command: {command_bytes}")
            self.arduino.write(command_bytes)
            
            # Stop the node
            self.destroy_node()
            rclpy.shutdown()
            print("Node stopping...")
            raise SystemExit("Received stop command, shutting down node.")

    def calculate_steering_angle(self, target_rot):
        # Calculate the tangent of the desired steering angle
        tanSteer = math.tan(target_rot)

        # Calculate the steering angle for the left wheel
        steering_angle_left = math.atan2(
            tanSteer, 1.0 - (self.wheel_separation / (2.0 * self.wheel_base)) * tanSteer
        )

        # Calculate the steering angle for the right wheel
        steering_angle_right = math.atan2(
            tanSteer, 1.0 + (self.wheel_separation / (2.0 * self.wheel_base)) * tanSteer
        )

        old_steering_angle = self.steering_angle
        
        # Calculate the average steering angle
        self.steering_angle = (steering_angle_left + steering_angle_right) / 2.0
        self.steering_angle = max(min(self.steering_angle, 0.74), -0.74)
        self.steering_angle = math.degrees(self.steering_angle)
        
        steering_angle = self.steering_angle - old_steering_angle

        return steering_angle

    def convert_twist_to_velocity_and_steering(self, twist_message):
        # Extract linear and angular velocities from the twist message
        target_linear = twist_message.linear.x
        target_rot = twist_message.angular.z

        # Calculate steering angle
        steering_angle = self.calculate_steering_angle(target_rot)
        print(steering_angle, target_linear)

        # Return linear velocity and steering angle
        return target_linear, steering_angle

    def send_data_to_arduino(self, steering_angle, linear_velocity):
        steering_angle = int(steering_angle * 497.84) * (-1)
        linear_velocity = linear_velocity * (18 / 5)
        
        # Pack the values with specified sizes
        command_bytes = struct.pack('<fi', linear_velocity, steering_angle)
        
        print(f"Sending command: {command_bytes}")
        
        self.arduino.write(command_bytes)
        time.sleep(0.75)

def main(args=None):
    rclpy.init(args=args)

    try:
        twist_converter_node = TwistConverterNode()
        rclpy.spin(twist_converter_node)
    except SystemExit as e:
        print(f"Shutting down: {e}")
    finally:
        if 'twist_converter_node' in locals():
            twist_converter_node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__':
    main()

