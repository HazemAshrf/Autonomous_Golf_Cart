#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes goal positions based on GPS 
coordinates received via a Telegram bot. It subscribes to live and non-live GPS 
locations sent to the bot, compares the goal GPS location with predefined 
locations and determines the closest target to send its corresponding (x, y, yaw).

Published Topics:
- /goal_pose (geometry_msgs/PoseStamped): Publishes the goal position and orientation.

Author: Hazem Ashraf
"""

import logging
import nest_asyncio
import threading
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix
from geometry_msgs.msg import PoseStamped
from pyproj import Proj
import math
from telegram import ForceReply, Update
from telegram.ext import Application, CommandHandler, ContextTypes, MessageHandler, filters

nest_asyncio.apply()

# Enable logging
logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)
logging.getLogger("httpx").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)

class GoalPublisher(Node):
    def __init__(self):
        super().__init__('goal_publisher')
        self.goal_pub_ = self.create_publisher(PoseStamped, 'goal_pose', 10)

        # Define the target locations with (x, y, heading) and corresponding GPS coordinates
        self.targets = [
            {"lat": 30.0052958, "lon": 31.7017616, "x": 198.80, "y": 52.18, "heading": -0.311},  #cafetria 
            {"lat": 30.0041298, "lon": 31.7017733, "x": 205.14, "y": -60.06, "heading": -2.190}, #essen
            {"lat": 30.0046310, "lon": 31.7019044, "x": 217.432 , "y": 7.795, "heading":  -1.633}, #food court
            {"lat": 30.0037634, "lon": 31.7007903, "x": 120.967, "y": -96.608 , "heading":  -2.788}, #M building
            {"lat": 30.0036914, "lon": 31.6997540, "x": 3.986, "y": -111.139 , "heading":  3.126}, #S building            
        ]

    def heading_to_quaternion(self, heading):
        # Calculate quaternion for heading
        qx = 0.0
        qy = 0.0
        qz = math.sin(heading / 2.0)
        qw = math.cos(heading / 2.0)
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

goal_publisher = None
live_period_timers = {}

def extract_coordinates_from_message(message):
    if message and message.location:
        latitude = message.location.latitude
        longitude = message.location.longitude
        return latitude, longitude
    return None, None

def determine_message_type(message):
    msg_type = 0
    if hasattr(message, "edit_date"):
        msg_type += 1
    if hasattr(message.location, "live_period"):
        msg_type += 2
    return msg_type

def handle_end_live_period(user, context):
    context.bot.send_message(user.id, "End of live period (automatically detected).")

async def location(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    user = update.effective_user
    message = update.edited_message if update.edited_message else update.message
    latitude, longitude = extract_coordinates_from_message(message)
    if latitude is None or longitude is None:
        return

    goal_publisher.set_goal_from_location(latitude, longitude)

    msg_type = determine_message_type(message)

    if msg_type == 0:
        await context.bot.send_message(user.id, f"Single (non-live) location update. Latitude: {latitude}, Longitude: {longitude}")
    elif msg_type == 1:
        await context.bot.send_message(user.id, "End of live period.")
        # Cancel the timer if it exists
        if user.id in live_period_timers:
            live_period_timers[user.id].cancel()
            del live_period_timers[user.id]
    elif msg_type == 2:
        await context.bot.send_message(user.id, f"Start of live period. Latitude: {latitude}, Longitude: {longitude}")
        # Schedule the end of the live period
        timer = threading.Timer(message.location.live_period, handle_end_live_period, args=(user, context))
        live_period_timers[user.id] = timer
        timer.start()
    elif msg_type == 3:
        await context.bot.send_message(user.id, f"Live location update. Latitude: {latitude}, Longitude: {longitude}")

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    user = update.effective_user
    await update.message.reply_html(
        rf"Hi {user.mention_html()}!",
        reply_markup=ForceReply(selective=True),
    )

async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    await update.message.reply_text("Help!")

def main() -> None:
    global goal_publisher
    rclpy.init()
    goal_publisher = GoalPublisher()

    application = Application.builder().token("YOUR_TELEGRAM_BOT_TOKEN").build()
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("help", help_command))
    application.add_handler(MessageHandler(filters.LOCATION, location))

    from threading import Thread
    def ros_spin():
        rclpy.spin(goal_publisher)
    ros_thread = Thread(target=ros_spin)
    ros_thread.start()

    application.run_polling(allowed_updates=Update.ALL_TYPES)
    goal_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()

