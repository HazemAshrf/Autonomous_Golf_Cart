import os
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, LogInfo, TimerAction
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    # Path to the directory containing your launch files
    dir_path = os.path.dirname(os.path.realpath(__file__))

    # Define the launch files to be included
    speed_limit_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(dir_path, 'speed_limit_launch_sim.py')),
    )

    keepout_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(dir_path, 'keepout_launch_sim.py')),
    )

    main_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(os.path.join(dir_path, 'golf_car_launch_sim.py')),  # Replace with your main launch file name
    )

    # Create the launch description
    ld = LaunchDescription()

    # Add the first launch file
    ld.add_action(speed_limit_launch)

    # Add a delay of 10 seconds before starting the second launch file
    ld.add_action(TimerAction(period=10.0, actions=[
        main_launch
    ]))

    # Add a delay of 10 seconds before starting the main launch file
    #ld.add_action(TimerAction(period=20.0, actions=[
      #  LogInfo(msg="Starting golf_car_launch_sim.py after another 10 seconds"),
     #   main_launch
    #]))

    return ld

