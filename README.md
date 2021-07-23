# IAC-LIME
This is the code that some of us at the Autonomous Ground Vehicle Research Group wrote for the Indy Autonomous Challenge for localisation in a mapped environment.
This repository contains the code which is used to map the environment from LIDAR data and generate a PCD file which can be viewed using PCD viewer or Open3D.
This code has been executed on a rosbag file which contains the data of a carla simulated environment.

In order to use this code :

- `git clone https://github.com/anubhabdg7/IAC-LIME`


- Now place the src,CMakeLists.txt and package.xml together in a folder and paste it in your ROS workspace(Do not forget to modify the src code by adding the topics being published by the rosbag file which you will be using)


- Now run `catkin_make carla_map` or `catkin build carla_map` inside your workspace src folder


- Source your terminal,run `roscore` and run `rosrun carla_map carla_map`.


- In another terminal run the rosbag file and you are done!


- This code will generate the map in a .pcd file format and save it in your workspace src folder.
