#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes goal positions based on GPS 
coordinates received via a Telegram bot. It subscribes to live and non-live GPS 
locations sent to the bot, converts the coordinates to a local Cartesian frame, 
transforms them to the map frame, and publishes the resulting goal positions 
as PoseStamped messages.

Note:
This code can only calculate the (x, y) for the goal location,
but the heading (yaw) needs to be set manually using the variable "self.goal_heading".

Published Topics:
- /goal_pose (geometry_msgs/PoseStamped): Publishes the goal position and orientation.

Author: Hazem Ashraf
"""

Published Topics:
- /goal_pose (geometry_msgs/PoseStamped): Publishes the goal position and orientation.

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

        # Heading for the goal location
        self.goal_heading = 90  # Example heading, replace with actual value
        
        self.angle_deg_ = 4.3  # The rotation angle between the map frame and the ENU frame
        self.lat_0 = 30.004695 # The latitude of the map frame
        self.lon_0 = 31.699729 # The longitude of the map frame
        self.local_cartesian_ = Proj(proj='aeqd', ellps='WGS84', datum='WGS84', lat_0=self.lat_0, lon_0=self.lon_0)

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

