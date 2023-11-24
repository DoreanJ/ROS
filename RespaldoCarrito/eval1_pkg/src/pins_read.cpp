#include <ros/ros.h>
#include <wiringPi.h>
#include <std_msgs/Int32.h>
#include <iostream>
#include <sstream>

int main (int argc, char **argv)
{
	// Pines encoder Izquierdo
    int I_pinA=26;
    int I_pinB=24;

    //Pines encoder Derecho
    int D_pinA=16;
    int D_pinB=18;
    
    //iniciamos nodod

    ros::init(argc, argv, "gpio_read_publisher");
    
    // generamos un objeto de de nodehandle
    ros::NodeHandle nh;
    
    //generacion de publicadores
    //ros::Publisher sw_status = nh.advertise<std_msgs::Int32>("/sw",10);
    //ros::Publisher pul_status = nh.advertise<std_msgs::Int32>("/pul",10);

    //Inicializacion de la libreria wiring PI
    //wiringPiSetup();
    wiringPiSetupPhys();

    // definir entradas para pines de encoder Izquierdo
    pinMode(I_pinA,INPUT);
    pinMode(I_pinB,INPUT);

    // definir entradas para pines de encoder Derecho
    pinMode(D_pinA,INPUT);
    pinMode(D_pinB,INPUT);
    
    
    

    

    ros::Rate loop_rate(10);

    while (ros::ok())
    {
    	std_msgs::Int32 statusw;
    	std_msgs::Int32 statupul;
        
        //lectura de pines

        int status_I_pinA=digitalRead(I_pinA);
        int status_I_pinB=digitalRead(I_pinB);
    
        int status_D_pinA=digitalRead(D_pinA);
        int status_D_pinB=digitalRead(D_pinB);
        
        
        std::cout<<"Pin A I:"<<status_I_pinA<<" Set as INPUT"<<"\n";
        std::cout<<"Pin B I:"<<status_I_pinB<<" Set as INPUT"<<"\n";
        std::cout<<"Pin A D:"<<status_D_pinA<<" Set as INPUT"<<"\n";
        std::cout<<"Pin B D:"<<status_D_pinB<<" Set as INPUT"<<"\n";


        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
