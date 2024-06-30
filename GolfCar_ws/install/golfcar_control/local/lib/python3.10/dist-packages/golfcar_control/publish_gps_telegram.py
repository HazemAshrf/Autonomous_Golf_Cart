#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes GPS coordinates received via a 
Telegram bot. It subscribes to live and non-live GPS locations sent to the bot, 
and publishes the resulting GPS coordinates as NavSatFix messages.

You can share live location with the telegram bot and use it as a GPS for the car

Published Topics:
- /fix (sensor_msgs/NavSatFix): Publishes the received GPS coordinates.

Author: Hazem Ashraf
"""

import logging
import nest_asyncio
import threading
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import NavSatFix
from telegram import ForceReply, Update
from telegram.ext import Application, CommandHandler, ContextTypes, MessageHandler, filters

nest_asyncio.apply()

# Enable logging
logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)
logging.getLogger("httpx").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)

class GPSPublisher(Node):
    def __init__(self):
        super().__init__('gps_publisher')
        self.publisher_ = self.create_publisher(NavSatFix, '/fix', 10)
        self.altitude = 228.0   # Published but not used in calculations
        self.frame_id = 'base_footprint'

    def publish_gps(self, latitude, longitude):
        msg = NavSatFix()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.header.frame_id = self.frame_id
        msg.latitude = latitude
        msg.longitude = longitude
        msg.altitude = self.altitude
        self.publisher_.publish(msg)
        self.get_logger().info(f'Published GPS coordinates: Latitude: {latitude}, Longitude: {longitude}, Altitude: {self.altitude}')

gps_publisher = None
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

    gps_publisher.publish_gps(latitude, longitude)

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
    global gps_publisher
    rclpy.init()
    gps_publisher = GPSPublisher()

    application = Application.builder().token("6474364812:AAGVJUMIzH9ig7Bma0u4rgLeld0uVihY4K4").build()     # YOUR_TELEGRAM_BOT_TOKEN
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("help", help_command))
    application.add_handler(MessageHandler(filters.LOCATION, location))

    from threading import Thread
    def ros_spin():
        rclpy.spin(gps_publisher)
    ros_thread = Thread(target=ros_spin)
    ros_thread.start()

    application.run_polling(allowed_updates=Update.ALL_TYPES)
    gps_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()

