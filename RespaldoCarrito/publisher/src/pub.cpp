#include <ros/ros.h>
#include <wiringPi.h>
#include <iostream>
#include <sstream>
#include <std_msgs/Int32.h>


#define LED_PIN 3 // change pin number here

int main (int argc, char **argv)
{
    ros::init(argc, argv, "pubgpio_ros");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<std_msgs::Int32>("/status",10);


    wiringPiSetupGpio();
    pinMode(LED_PIN, INPUT);
    ROS_INFO("GPIO has been set as INPUT.");

    ros::Rate loop_rate(3);

    while (ros::ok())
    {

        int value= digitalRead(LED_PIN);

        if (value==0){
        	std_msgs::Int32 val;
        	val.data=5;
        	pub.publish(val);

        	std::cout<<"Published"<<val.data<<"\n";
        }

        ros::spinOnce();
        loop_rate.sleep();

    }

    return 0;
}
