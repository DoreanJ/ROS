/**
 * @author  Dwindra Sulistyoutomo
 */
#include <ros/ros.h>
#include <iostream>	
#include <iomanip>
#include <fstream>
#include <chrono>       // Required to get time
#include <sensor_msgs/Imu.h>

#include <MPU6050Pi.h>

#define COMPLEMENTARY_FILTER_CONSTANT   0.98

float ComplementaryFilter(float angle, float angle_comp) {
    return COMPLEMENTARY_FILTER_CONSTANT * angle + (1- COMPLEMENTARY_FILTER_CONSTANT) * angle_comp;
}


int main (int argc, char **argv)
{


    ros::init(argc, argv, "gpio_read_mpu6050");
    ros::NodeHandle nh;
    ros::Publisher imu_pub = nh.advertise<sensor_msgs::Imu>("/imu_mpu6050",10);

    // Connect to device with default setting
    MPU6050Pi mpu;
    std::cout<<"Current configuration: Accel "<<mpu.GetAccelSensitivity()<<" Gyro: "<<mpu.GetGyroSensitivity()<<std::endl;


    //Ros params
    double acc_x_offset = 0;
    double acc_y_offset = 0;
    double acc_z_offset = 0;

    double gyr_x_offset = 0;
    double gyr_y_offset = 0;
    double gyr_z_offset = 0;


    nh.param<double>("acceleration_x_offset",acc_x_offset,0);
    nh.param<double>("acceleration_y_offset",acc_y_offset,0);
    nh.param<double>("acceleration_z_offset",acc_z_offset,0);

    nh.param<double>("gyro_x_offset",gyr_x_offset,0);
    nh.param<double>("gyro_y_offset",gyr_y_offset,0);
    nh.param<double>("gyro_z_offset",gyr_z_offset,0);


    int16_t ax, ay, az, gx, gy, gz;
    float accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z;


	std::cout << "Float Sensor Data\n";
	std::cout << std::fixed << std::setprecision(6) << std::setfill(' ');
	std::cout << std::setw(10) << "X(g)" << std::setw(10) << "Y(g)" << std::setw(10) << "Z(g)";
	std::cout << std::setw(10) << "X(deg/s)" << std::setw(10) << "Y(deg/s)" << std::setw(10) << "Z(deg/s)";
	std::cout << std::endl;
	// Publish in loop.
	
		   while(1) {
		       // Get gyroscope data.
		       mpu.GetGyroFloat(&gyro_x, &gyro_y, &gyro_z);
		       // Get accelerometer values.
		       mpu.GetAccelFloat(&accel_x, &accel_y, &accel_z);



		       sensor_msgs::Imu imu_data;
		       imu_data.header.stamp = ros::Time::now();
		       imu_data.header.frame_id = "map";

		       imu_data.linear_acceleration.x = accel_x;
		       imu_data.linear_acceleration.y = accel_y;
		       imu_data.linear_acceleration.z = accel_z;

		       imu_data.angular_velocity.x = gyro_x;
		       imu_data.angular_velocity.y = gyro_y;
		       imu_data.angular_velocity.z = gyro_z;

		       imu_pub.publish(imu_data);

		       std::cout << std::setw(10) << accel_x << std::setw(10) << accel_y << std::setw(10) << accel_z;
		       std::cout << std::setw(10) << gyro_x << std::setw(10) << gyro_y << std::setw(10) << gyro_z;
		       std::cout << "\r";
		   }

    

    return 0;
}
