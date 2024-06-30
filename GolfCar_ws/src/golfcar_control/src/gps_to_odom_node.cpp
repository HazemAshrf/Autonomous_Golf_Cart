#include "gps_to_odom_node.hpp"

GpsToOdomNode::GpsToOdomNode()
    : Node("gps_to_odom_node"),
      local_cartesian_(0.0, 0.0, 0.0),
      initial_position_set_(false),
      angle_deg_(180) // Example angle, replace with actual value
{
    gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>(
        "/gps/fix", 10, std::bind(&GpsToOdomNode::gpsCallback, this, std::placeholders::_1));
    
    odom_pub_ = this->create_publisher<nav_msgs::msg::Odometry>("/odometry/gps", 10);

    RCLCPP_INFO(this->get_logger(), "Node initialized. Waiting for first GPS fix to set initial position.");
}

void GpsToOdomNode::transformENUtoOdom(double enu_x, double enu_y, double angle_deg, double& odom_x, double& odom_y)
{
    // Convert angle from degrees to radians
    double angle_rad = angle_deg * M_PI / 180.0;

    // Calculate the rotation matrix components
    double cos_theta = std::cos(angle_rad);
    double sin_theta = std::sin(angle_rad);

    // Apply the rotation matrix
    odom_x = enu_x * cos_theta - enu_y * sin_theta;
    odom_y = enu_x * sin_theta + enu_y * cos_theta;
}

void GpsToOdomNode::gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg)
{
    if (msg->status.status == sensor_msgs::msg::NavSatStatus::STATUS_NO_FIX)
    {
        RCLCPP_WARN(this->get_logger(), "No GPS fix available.");
        return;
    }

    if (!initial_position_set_)
    {
        local_cartesian_.Reset(msg->latitude, msg->longitude, msg->altitude);
        initial_position_set_ = true;
        RCLCPP_INFO(this->get_logger(), "Initial position set to (lat, lon): (%f, %f)", msg->latitude, msg->longitude);
    }

    double enu_x, enu_y, enu_z;
    local_cartesian_.Forward(msg->latitude, msg->longitude, msg->altitude, enu_x, enu_y, enu_z);

    double odom_x, odom_y;
    transformENUtoOdom(enu_x, enu_y, angle_deg_, odom_x, odom_y);

    auto odom_msg = nav_msgs::msg::Odometry();
    odom_msg.header.stamp = msg->header.stamp;
    odom_msg.header.frame_id = "odom";
    odom_msg.child_frame_id = "base_footprint";

    odom_msg.pose.pose.position.x = odom_x;
    odom_msg.pose.pose.position.y = odom_y;
    odom_msg.pose.pose.position.z = 0.0;
    odom_msg.pose.pose.orientation.x = 0.0;
    odom_msg.pose.pose.orientation.y = 0.0;
    odom_msg.pose.pose.orientation.z = 0.0;
    odom_msg.pose.pose.orientation.w = 1.0;

    for (size_t i = 0; i < 36; ++i) {
        odom_msg.pose.covariance[i] = 0.0;
    }

    odom_pub_->publish(odom_msg);

    RCLCPP_INFO(this->get_logger(), "Published odometry: x=%f, y=%f", odom_x, odom_y);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GpsToOdomNode>());
    rclcpp::shutdown();
    return 0;
}
