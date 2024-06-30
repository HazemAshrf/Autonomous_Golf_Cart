from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            name='scanner', default_value='scanner',
            description='Namespace for sample topics'
        ),
        Node(
            package='tf2_ros',
            executable='static_transform_publisher',
            name='static_transform_publisher',

        ),
        Node(
            package='pointcloud_to_laserscan', executable='pointcloud_to_laserscan_node',
            #remappings=[('cloud_in', '/velodyne_points')],
            parameters=[{
                'target_frame': 'laser_frame',
                'transform_tolerance': 0.01,
                'min_height': -1.7,
                'max_height': 0.0,
                'angle_min': -1.5708,  # -M_PI/2
                'angle_max': 1.5708,  # M_PI/2
                'angle_increment':  0.002094,  # M_PI/360.0   0.0087 0.002094
                'scan_time': 0.03333,
                'range_min': 0.45,
                'range_max': 100.0,
                'use_inf': True,
                'inf_epsilon': 1.0
            }],
            name='pointcloud_to_laserscan'
        )
    ])
    
    
    
    #('cloud_in', [LaunchConfiguration(variable_name='scanner'), '/cloud']),
                      #  ('scan', [LaunchConfiguration(variable_name='scanner'), '/scan'])
