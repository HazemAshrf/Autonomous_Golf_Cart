#include "orientation_calculator_node.hpp"

OrientationCalculatorNode::OrientationCalculatorNode()
    : Node("orientation_calculator_node"),
      local_cartesian_(0.0, 0.0, 0.0),
      initial_position_set_(false),
      second_position_set_(false)
{
    gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>(
        "/fix", 10, std::bind(&OrientationCalculatorNode::gpsCallback, this, std::placeholders::_1));
    
    RCLCPP_INFO(this->get_logger(), "Node initialized. Waiting for GPS fixes to calculate orientation.");
}

void OrientationCalculatorNode::gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
    if (msg->status.status == sensor_msgs::msg::NavSatStatus::STATUS_NO_FIX)
    {
        RCLCPP_WARN(this->get_logger(), "No GPS fix available.");
        return;
    }

    if (!initial_position_set_)
    {
        initial_lat_ = msg->latitude;
        initial_lon_ = msg->longitude;
        local_cartesian_.Reset(msg->latitude, msg->longitude, msg->altitude);
        initial_position_set_ = true;
        RCLCPP_INFO(this->get_logger(), "Initial position set to (lat, lon): (%f, %f)", msg->latitude, msg->longitude);
    }
    else if (!second_position_set_)
    {
        double x, y, z;
        local_cartesian_.Forward(msg->latitude, msg->longitude, msg->altitude, x, y, z);

        double delta_x = x;
        double delta_y = y;
        double distance = std::sqrt(delta_x * delta_x + delta_y * delta_y);

        double angle = std::atan2(delta_y, delta_x);
        RCLCPP_INFO(this->get_logger(), "Second position set to (lat, lon): (%f, %f)", msg->latitude, msg->longitude);
        RCLCPP_INFO(this->get_logger(), "Orientation angle between odom and ENU frame: %f degrees", angle * 180.0 / M_PI);
        RCLCPP_INFO(this->get_logger(), "Distance moved: %f meters", distance);

        second_position_set_ = true;
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<OrientationCalculatorNode>());
    rclcpp::shutdown();
    return 0;
}

