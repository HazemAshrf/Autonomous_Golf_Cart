#ifndef GPS_TO_ODOM_NODE_HPP
#define GPS_TO_ODOM_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <cmath>

class GpsToOdomNode : public rclcpp::Node
{
public:
    GpsToOdomNode();

private:
    void gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);
    void transformENUtoOdom(double enu_x, double enu_y, double angle_deg, double& odom_x, double& odom_y);

    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr odom_pub_;
    GeographicLib::LocalCartesian local_cartesian_;
    bool initial_position_set_;
    double angle_deg_; // Angle between ENU and odom frames
};

#endif // GPS_TO_ODOM_NODE_HPP
