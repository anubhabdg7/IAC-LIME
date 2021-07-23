#include <iostream>
#include <pcl/point_types.h>
#include <pcl/common/io.h>
#include <pcl/io/pcd_io.h>
#include <ros/ros.h>
#include <pcl_ros/transforms.h>
#include <pcl/common/transforms.h>
#include <pcl_ros/point_cloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <tf2_msgs/TFMessage.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>                     //Including all the necessary header files and libraries
#include <cstdint>
#include <nav_msgs/Odometry.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl_conversions/pcl_conversions.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>



pcl::PointCloud<pcl::PointXYZRGBA> final_cloud,curr_cloudXYZ;      //Defining the point clouds


ros::Publisher tf_publisher;                                  

int i=0;


void tf_callback(const tf2_msgs::TFMessage::ConstPtr& tf){
	tf2_msgs::TFMessage tf_ = *tf;
	tf_publisher.publish(tf_.transforms[0]);                                 //This takes in /tf and converts it into /tf_data
}


	
}
void pcl_callback(const sensor_msgs::PointCloud2::ConstPtr& pcl_cloud,const geometry_msgs::TransformStamped::ConstPtr& tf){
	
	geometry_msgs::TransformStamped tf2=*tf;
	geometry_msgs::Transform tf_ = tf2.transform;
    sensor_msgs::PointCloud2 curr;
    pcl_ros::transformPointCloud("map",tf_,*pcl_cloud,curr);
    pcl::PCLPointCloud2 curr_cloud;
    pcl_conversions::toPCL(curr, curr_cloud);
    
    pcl::fromPCLPointCloud2(curr_cloud, curr_cloudXYZ);
    final_cloud += curr_cloudXYZ;
	ROS_INFO_STREAM("Transform added to final cloud."<<i);
	i++;


	
	
}
int main(int argc, char** argv)
{
	ros::init(argc, argv, "stitch_pcd");
	ros::NodeHandle nh;
	
	tf_publisher = nh.advertise<geometry_msgs::TransformStamped>("/tf_data",1);
	ros::Subscriber tf_subscriber1 = nh.subscribe("/tf", 1, tf_callback);
	ros::Subscriber odom_subs=nh.subscribe("/IndyOdom/noisy_odom",1,odom_callback);
	ros::Subscriber odom_subs1=nh.subscribe("/carla/vehicle/086/odometry",1,odom_callback1);

	

    message_filters::Subscriber<sensor_msgs::PointCloud2> pcl_subscriber(nh, "/carla/vehicle/086/lidar/front/point_cloud", 1);   
	message_filters::Subscriber<geometry_msgs::TransformStamped> tf_subscriber(nh, "/tf_data", 1);                                             //Time synchronization
	message_filters::TimeSynchronizer<sensor_msgs::PointCloud2, geometry_msgs::TransformStamped> sync(pcl_subscriber, tf_subscriber, 10);  
    sync.registerCallback(boost::bind(&pcl_callback, _1, _2));



	
	
	ros::spin();
	


	pcl::io::savePCDFileASCII("localised_map_temp_1.pcd", final_cloud); 
	return 0;
}
