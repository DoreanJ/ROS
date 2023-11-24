/*
 *  This code will subscriber integer values from demo_topic_publisher
*/


#include "ros/ros.h"
#include <geometry_msgs/Vector3.h>
#include <sensor_msgs/Imu.h>
#include <iostream>
#include <sstream>


void rpm_callback(const geometry_msgs::Vector3::ConstPtr & msg)
{
	int rpmI = msg->x;
	int rpmD = msg->y;
	std::cout<<"\tVelocidades de cada Motor"<<"\n";
	std::cout<<" RPM motor Izquierdo:"<<rpmI<<"\n";
	std::cout<<" RPM motor Derecho:"<<rpmD<<"\n";
	std::cout<<"---------------------------------------------------"<<"\n";

}

void imu_callback(const sensor_msgs::Imu::ConstPtr & msg)
{
	float Ac_L_x = msg->linear_acceleration.x;
	float Ac_L_y = msg->linear_acceleration.y;
	float Ac_L_z = msg->linear_acceleration.z;
	float Ac_An_x = msg->angular_velocity.x;
	float Ac_An_y = msg->angular_velocity.y;
	float Ac_An_z  = msg->angular_velocity.z;


	std::cout<<"\tAceleraciones lineales:"<<"\n";
	std::cout<<" X:"<<Ac_L_x<<"\n";
	std::cout<<" Y:"<<Ac_L_y<<"\n";
	std::cout<<" Z:"<<Ac_L_z<<"\n";
	std::cout<<"\tVelocidades angulares:"<<"\n";
	std::cout<<" X:"<<Ac_An_x<<"\n";
	std::cout<<" Y:"<<Ac_An_y<<"\n";
	std::cout<<" Z:"<<Ac_An_z<<"\n";
	std::cout<<"---------------------------------------------------"<<"\n";

}


int main(int argc, char **argv)

{

	//Initializing ROS node with a name of demo_topic_subscriber
	ros::init(argc, argv,"demo_std_msg_subscriber");
	//Created a nodehandle object
	ros::NodeHandle nj;
	//Create a publisher object
	ros::Subscriber rpm_subscriber = nj.subscribe("/rpm",10,rpm_callback);
	
	ros::Subscriber imu_subscriber = nj.subscribe("/imu_mpu6050",10,imu_callback);
	
	
	//Spinning the node
	ros::spin();
	return 0;
}


