import os
from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import ExecuteProcess, DeclareLaunchArgument
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution

def generate_launch_description():
    # Define the path to the Xacro and world files relative to the launch file
    dir_path = os.path.dirname(os.path.realpath(__file__))
    xacro_file_path = os.path.join(dir_path, '..', 'urdf', 'golf_car.xacro')
    world_file_path = os.path.join(dir_path, '..', 'worlds', 'no_robot_world')

    # Declare the use of simulation time
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation time if true'
    )

    return LaunchDescription([
        # Start Gazebo server with the specified world
        ExecuteProcess(
            cmd=['gazebo', '--verbose', world_file_path, '-s', 'libgazebo_ros_factory.so'],
            output='screen'
        ),

        use_sim_time_arg,

        # Spawn your robot into Gazebo
        Node(
            package='gazebo_ros', executable='spawn_entity.py',
            arguments=['-topic', 'robot_description', '-entity', 'car_robot'],
            output='screen'
        ),

        # Provide the robot description (generated from the Xacro file)
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            output='screen',
            parameters=[{
                'use_sim_time': LaunchConfiguration('use_sim_time'),
                'robot_description': Command(['xacro ', xacro_file_path])
            }],
        )
    ])

