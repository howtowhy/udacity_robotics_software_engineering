#!/bin/bash

## deploy a turtlebot in my environment
xterm -e "roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(pwd)/../map/Udacity_Office.world" &
sleep 10


