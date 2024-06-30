#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes GPS coordinates received via a Flask web server. 
The server listens for incoming location data via POST requests and publishes the received coordinates 
as NavSatFix messages. It also provides a GET endpoint to retrieve the latest location data.

Published Topics:
- /fix (sensor_msgs/NavSatFix): Publishes the received GPS coordinates.

Flask Endpoints:
- POST /location: Receives and publishes GPS coordinates (latitude, longitude, altitude).
- GET /get_location: Returns the latest published GPS coordinates.

Expected Location Message Format:
The POST request to /location should contain a JSON object with the following fields:
{
    "Latitude": "latitude_value",
    "Longitude": "longitude_value",
    "Altitude": "altitude_value"
}

Usage Instructions:
1. Setup a shortcut on your iPhone to get the current location and send it to the Flask server:
    a. Create a new Shortcut on your iPhone.
    b. Add the "Get Current Location" action.
    c. Add the "Get Contents of URL" action:
        - URL: http://<server_ip>:5000/location
        - Method: POST
        - Headers: {"Content-Type": "application/json"}
        - Request Body: {"Latitude": "<latitude>", "Longitude": "<longitude>", "Altitude": "<altitude>"}
    d. Save the shortcut.

2. Use Scriptable to repeatedly execute the shortcut:
    a. Create a new script in the Scriptable app.
    b. Add the following code to the script:
        ```
				let interval = 100; // Interval in milliseconds
				let shortcutName = "Send_GPS"; // Replace with the name of your shortcut

				let timer = Timer.schedule(interval, true, async () => {
					let shortcut = new CallbackURL("shortcuts://run-shortcut");
					shortcut.addParameter("name", shortcutName);
					await shortcut.open();
				});

				let alert = new Alert();
				alert.title = "Shortcut Runner";
				alert.message = "Running your shortcut every 1 seconds. Tap Stop to end.";
				alert.addAction("Stop");

				await alert.presentAlert();
				timer.invalidate(); // Stops the timer when alert is dismissed
        ```
    c. Save and run the script. This will periodically run the shortcut and send GPS data to the server.

With this setup, your iPhone will act as the GPS for the car, continuously sending location data to the server, which then publishes it as ROS2 messages.

Author: Hazem Ashraf
"""

import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix
from flask import Flask, request, jsonify
import threading

app = Flask(__name__)

location_data = {}

class GPSPublisher(Node):
    def __init__(self):
        super().__init__('gps_publisher')
        self.publisher_ = self.create_publisher(NavSatFix, '/fix', 10)
    
    def publish_gps_data(self, latitude, longitude, altitude):
        msg = NavSatFix()
        msg.latitude = float(latitude)
        msg.longitude = float(longitude)
        msg.altitude = float(altitude.replace(',', '.'))
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: Latitude: {latitude}, Longitude: {longitude}, Altitude: {altitude}')

def ros2_thread():
    rclpy.init()
    global gps_publisher
    gps_publisher = GPSPublisher()
    rclpy.spin(gps_publisher)
    gps_publisher.destroy_node()
    rclpy.shutdown()

@app.route('/location', methods=['POST'])
def update_location():
    global location_data
    data = request.json
    latitude = data.get('Latitude')
    longitude = data.get('Longitude')
    altitude = data.get('Altitude')
    
    location_data = {
        'latitude': latitude,
        'longitude': longitude,
        'altitude': altitude
    }
    
    gps_publisher.publish_gps_data(latitude, longitude, altitude)
    
    return jsonify({"status": "success"}), 200

@app.route('/get_location', methods=['GET'])
def get_location():
    return jsonify(location_data), 200

if __name__ == '__main__':
    ros2_thread = threading.Thread(target=ros2_thread)
    ros2_thread.start()
    app.run(host='0.0.0.0', port=5000)

