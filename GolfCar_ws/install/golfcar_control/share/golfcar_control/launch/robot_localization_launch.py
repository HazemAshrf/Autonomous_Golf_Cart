from launch import LaunchDescription
from ament_index_python.packages import get_package_share_directory
import launch_ros.actions
import os
import yaml
from launch.substitutions import EnvironmentVariable
import pathlib
import launch.actions
from launch.actions import DeclareLaunchArgument

dir_path = os.path.dirname(os.path.realpath(__file__))

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            package='robot_localization',
            executable='navsat_transform_node',
            name='navsat_transform_node',
            output='screen',
            parameters=[os.path.join(dir_path, '..', 'config', 'navsat_transform.yaml')],
            remappings=[
                ('/gps/fix', '/fix'),
                ('/odometry/filtered', '/odom')
            ]
           ),
])

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            package='robot_localization',
            executable='ekf_node',
            name='ekf_filter_node',
            output='screen',
            parameters=[os.path.join(dir_path, '..', 'config', 'ekf.yaml')],
           ),
])
