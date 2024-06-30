#!/usr/bin/env python3

"""
Description:
This script implements a ROS2 node that publishes a stop signal to the '/stop_car' topic 
when receiving a specific message ("1") via a Telegram bot. The node integrates with 
Telegram to handle incoming messages and trigger the stop signal.

Published Topics:
- /stop_car (std_msgs/Int32): Publishes a stop signal (value 1) to stop the vehicle.

Author: Hazem Ashraf
"""

import logging
import nest_asyncio
import threading
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int32
from telegram import ForceReply, Update
from telegram.ext import Application, CommandHandler, ContextTypes, MessageHandler, filters

nest_asyncio.apply()

# Enable logging
logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s", level=logging.INFO
)
logging.getLogger("httpx").setLevel(logging.WARNING)
logger = logging.getLogger(__name__)

class StopCarPublisher(Node):
    def __init__(self):
        super().__init__('stop_car_publisher')
        self.publisher_ = self.create_publisher(Int32, '/stop_car', 10)

    def publish_stop_signal(self):
        msg = Int32()
        msg.data = 1
        self.publisher_.publish(msg)
        self.get_logger().info('Published stop signal: 1')

stop_car_publisher = None

async def handle_message(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    user = update.effective_user
    message = update.message.text
    if message == "1":
        stop_car_publisher.publish_stop_signal()
        await context.bot.send_message(user.id, "Stop signal sent to the car.")

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    user = update.effective_user
    await update.message.reply_html(
        rf"Hi {user.mention_html()}!",
        reply_markup=ForceReply(selective=True),
    )

async def help_command(update: Update, context: ContextTypes.DEFAULT_TYPE) -> None:
    await update.message.reply_text("Help!")

def main() -> None:
    global stop_car_publisher
    rclpy.init()
    stop_car_publisher = StopCarPublisher()

    application = Application.builder().token("6474364812:AAGVJUMIzH9ig7Bma0u4rgLeld0uVihY4K4").build()
    application.add_handler(CommandHandler("start", start))
    application.add_handler(CommandHandler("help", help_command))
    application.add_handler(MessageHandler(filters.TEXT & (~filters.COMMAND), handle_message))

    from threading import Thread
    def ros_spin():
        rclpy.spin(stop_car_publisher)
    ros_thread = Thread(target=ros_spin)
    ros_thread.start()

    application.run_polling(allowed_updates=Update.ALL_TYPES)
    stop_car_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()

