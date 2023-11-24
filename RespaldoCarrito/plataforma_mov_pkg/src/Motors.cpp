/*
 *  This code will subscriber integer values from demo_topic_publisher
 */

#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <sstream>
#include <xlsxwriter.h>

int range = 255;

int sentido;
int stop;

int L;

struct Data
{
	int rpmI;
	int rpmD;
	int pwmMI;
	int pwmMD;
	double time;
};

std::vector<Data> data;

// definicion de pines para motor 1
#define MD_IN1 5 // Pin In1 Motor Derecho
#define MD_IN2 6 // Pin In2 Motor Derecho

#define MD_ENA 13 // pin del pwm Motor Derecho

// definicion de pines para motor 2

#define MI_IN1 17 // Pin In1 Motor Izquierdo
#define MI_IN2 27 // Pin In2 Motor Izquierdo

#define MI_ENA 12 // pin del pwm Motor Izquierdo

// Función para guardar datos en el archivo Excel
void saveToExcel(const char *filename)
{
	lxw_workbook *workbook = workbook_new(filename);
	lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);

	// Escribir encabezados
	worksheet_write_string(worksheet, 0, 0, "Tiempo", NULL);
	worksheet_write_string(worksheet, 0, 1, "RPM Motor Izquierdo", NULL);
	worksheet_write_string(worksheet, 0, 2, "RPM Motor Derecho", NULL);
	worksheet_write_string(worksheet, 0, 3, "PWM Motor Izquierdo", NULL);
	worksheet_write_string(worksheet, 0, 4, "PWM Motor Derecho", NULL);

	// Escribir datos
	for (size_t i = 0; i < data.size(); ++i)
	{
		worksheet_write_number(worksheet, i + 1, 0, data[i].time, NULL);
		worksheet_write_number(worksheet, i + 1, 1, data[i].rpmI, NULL);
		worksheet_write_number(worksheet, i + 1, 2, data[i].rpmD, NULL);
		worksheet_write_number(worksheet, i + 1, 3, data[i].pwmMI, NULL);
		worksheet_write_number(worksheet, i + 1, 4, data[i].pwmMD, NULL);
	}

	workbook_close(workbook);
}

void callback(const geometry_msgs::Twist::ConstPtr &msg)
{

	float v = msg->linear.x;
	float w = msg->angular.z;

	// ecuacion que describe el movimiento del carro
	int VL = static_cast<int>(v - w);
	int VR = static_cast<int>(v + w);

	// Adecuacion de limites
	if (VL > 255)
	{
		VL = 255;
	}
	if (VL < -255)
	{
		VL = -255;
	}
	if (VR > 255)
	{
		VL = 255;
	}
	if (VR < -255)
	{
		VL = -255;
	}

	if (VL > 0)
	{
		digitalWrite(MI_IN1, HIGH);
		digitalWrite(MI_IN2, LOW);
	}
	else if (VL < 0)
	{
		digitalWrite(MI_IN1, LOW);
		digitalWrite(MI_IN2, HIGH);
	}

	if (VR > 0)
	{
		digitalWrite(MD_IN1, HIGH);
		digitalWrite(MD_IN2, LOW);
	}
	else if (VR < 0)
	{
		digitalWrite(MD_IN1, LOW);
		digitalWrite(MD_IN2, HIGH);
	}

	softPwmWrite(MD_ENA, abs(VR));
	softPwmWrite(MI_ENA, abs(VL));

	// std::cout << "pwm MD set in " << VR << "\n";
	// std::cout << "pwm MI set in " << VL << "\n";
	// Agregar datos al vector
	Data currentData;
	currentData.time = ros::Time::now().toSec();
	currentData.rpmI = pulse_count_encI; // O cualquier variable que almacene la velocidad del motor izquierdo
	currentData.rpmD = pulse_count_encD; // O cualquier variable que almacene la velocidad del motor derecho
	currentData.pwmMI = abs(VL);
	currentData.pwmMD = abs(VR);
	data.push_back(currentData);
}

int main(int argc, char **argv)

{
	// Inicio de Ros
	ros::init(argc, argv, "subs_teleop");

	// Created a nodehandle object

	ros::NodeHandle node_obj;

	// Crear subscriber para cada GPIO

	ros::Subscriber teleop_subscriber = node_obj.subscribe("/cmd_vel", 10, callback);

	// Configuracion de pines

	wiringPiSetupGpio();
	ROS_INFO("GPIO has been set as OUTPUT.");

	pinMode(MD_ENA, OUTPUT);
	pinMode(MD_IN1, OUTPUT);
	pinMode(MD_IN2, OUTPUT);

	pinMode(MI_ENA, OUTPUT);
	pinMode(MI_IN1, OUTPUT);
	pinMode(MI_IN2, OUTPUT);

	// inicializamos al pin como pwm con un rango de 0 a 255
	softPwmCreate(MD_ENA, 0, range);
	softPwmCreate(MI_ENA, 0, range);

	// Spinning the node
	saveToExcel("datos_motor.xlsx");
	ros::spin();

	return 0;
}
