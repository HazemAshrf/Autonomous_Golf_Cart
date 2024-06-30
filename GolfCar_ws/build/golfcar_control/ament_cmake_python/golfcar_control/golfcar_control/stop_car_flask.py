#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes a stop signal to the 'stop_car' topic when receiving a specific message ("Stop") via a Flask web server. The server listens for incoming messages via POST or GET requests and triggers the stop signal when the message is received. The script also uses LocalTunnel to expose the Flask server to the internet.

Published Topics:
- /stop_car (std_msgs/Int32): Publishes a stop signal (value 1) to stop the vehicle.

Flask Endpoints:
- GET / POST /message: Receives a message and triggers the stop signal if the message is "Stop".

Send a POST or GET request with the message "Stop" to the Flask server to publish the stop signal to the 'stop_car' topic.

Author: Hazem Ashraf
"""

import requests
from flask import Flask, request, jsonify
from threading import Thread
import subprocess
import time
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32

app = Flask(__name__)

class StopCarPublisher(Node):
    def __init__(self):
        super().__init__('stop_car_publisher')
        self.publisher_ = self.create_publisher(Int32, 'stop_car', 10)

    def publish_stop_signal(self):
        msg = Int32()
        msg.data = 1
        self.publisher_.publish(msg)
        self.get_logger().info('Published stop signal')

stop_car_publisher = None

@app.route('/message', methods=['GET', 'POST'])
def receive_message():
    if request.method == 'POST':
        data = request.get_json()
    else:  # GET request
        data = request.args.to_dict()
    print("Message received:", data)
    if 'message' in data and data['message'] == 'Stop':
        stop_car_publisher.publish_stop_signal()
    return jsonify({"status": "success", "message": "Message received"}), 200

def run_server():
    print("Starting Flask server...")
    app.run(port=5000, use_reloader=False)

def start_localtunnel():
    print("Starting LocalTunnel...")
    process = subprocess.Popen(["lt", "--port", "5000", "--subdomain", "yoursubdomain"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    
    while True:
        output = process.stdout.readline()
        if output == '' and process.poll() is not None:
            break
        if output:
            print(output.strip())  # Print the output for debugging

def ros2_init():
    global stop_car_publisher
    rclpy.init()
    stop_car_publisher = StopCarPublisher()
    rclpy.spin(stop_car_publisher)
    stop_car_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    try:
        # Start the ROS2 node in a separate thread
        ros2_thread = Thread(target=ros2_init)
        ros2_thread.daemon = True
        ros2_thread.start()

        # Start the Flask server in a separate thread
        server_thread = Thread(target=run_server)
        server_thread.daemon = True
        server_thread.start()

        # Start LocalTunnel
        start_localtunnel()

        # Keep the main thread running
        while True:
            time.sleep(1)
    except Exception as e:
        print(f"An error occurred: {e}")
    except KeyboardInterrupt:
        print("Shutting down server...")
        server_thread.join()

