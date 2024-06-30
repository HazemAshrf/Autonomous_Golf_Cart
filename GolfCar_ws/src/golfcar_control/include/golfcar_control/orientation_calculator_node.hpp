#ifndef ORIENTATION_CALCULATOR_NODE_HPP
#define ORIENTATION_CALCULATOR_NODE_HPP

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <GeographicLib/LocalCartesian.hpp>
#include <cmath>

class OrientationCalculatorNode : public rclcpp::Node
{
public:
    OrientationCalculatorNode();

private:
    void gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg);

    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
    GeographicLib::LocalCartesian local_cartesian_;
    bool initial_position_set_;
    bool second_position_set_;
    double initial_lat_;
    double initial_lon_;
};

#endif // ORIENTATION_CALCULATOR_NODE_HPP

