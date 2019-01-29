#include "ros/ros.h"
//#include "geometry_msgs/Vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "pubtrial2/Num.h"

#define MPU6050_GYRO_XOUT_H        0x43   // R
#define MPU6050_GYRO_YOUT_H        0x45   // R
#define MPU6050_GYRO_ZOUT_H        0x47   // R

#define MPU6050_ACCEL_XOUT_H       0x3B   // R
#define MPU6050_ACCEL_YOUT_H       0x3D   // R
#define MPU6050_ACCEL_ZOUT_H       0x3F   // R
 
#define MPU6050_PWR_MGMT_1         0x6B   // R/W
#define MPU6050_I2C_ADDRESS        0x68   // I2C

struct dummyvar
{

	float Gx;
	float Gy;
	float Gz;	
	float Ax;
	float Ay;
	float Az;
} var1;


int init(){
int fd = wiringPiI2CSetup(MPU6050_I2C_ADDRESS);
	wiringPiI2CReadReg8 (fd, MPU6050_PWR_MGMT_1);
    wiringPiI2CWriteReg16(fd, MPU6050_PWR_MGMT_1, 0);
    if (fd == -1)
        exit(0);

return fd;
}


dummyvar accelerometerfunc(int fd)
{
    
 
    float x,y,z,a,b,c;
 
        x = wiringPiI2CReadReg8(fd, MPU6050_GYRO_XOUT_H);
        y = wiringPiI2CReadReg8(fd, MPU6050_GYRO_YOUT_H);
        z = wiringPiI2CReadReg8(fd, MPU6050_GYRO_ZOUT_H);

		a = wiringPiI2CReadReg8(fd, MPU6050_ACCEL_XOUT_H );
        b = wiringPiI2CReadReg8(fd, MPU6050_ACCEL_YOUT_H );
        c = wiringPiI2CReadReg8(fd, MPU6050_ACCEL_ZOUT_H );
 
        //printf("x=%f \n  y=%f \n  z=%f\n a=%f\n b=%f\n c=%f\n", x,y,z,a,b,c); 
	dummyvar var={x,y,z,a,b,c};

	return var;
}


int main(int argc, char **argv)
{

	ros::init(argc, argv, "pubtrial2");
	ros::NodeHandle n;
	ros::Publisher puboutput_pub2 = n.advertise<pubtrial2::Num>("/puboutput2", 1);
	ros::Rate loop_rate(1000);

	int fd = init();
	if (fd < 0)
	{
		std::cerr << "FAILED" << std::endl;
	}
	

	while (ros::ok())
	{

/*
		geometry_msgs::Vector3 msg1;
		geometry_msgs::Vector3 msg2;
		float temp1,temp2, temp3,temp4,temp5,temp6;
		dummyvar var2;
		var2=accelerometerfunc();

		temp1=var2.Gx;
		temp2=var2.Gy;
		temp3=var2.Gz;
		temp4=var2.Ax;
		temp4=var2.Ay;
		temp4=var2.Az;

		msg1.x=temp1;
		msg1.y=temp2;
		msg1.z=temp3;
		msg2.x=temp4;
		msg2.y=temp5;
		msg2.z=temp6;

	    ROS_INFO("%f", temp1);

		puboutput_pub2.publish(msg1);
		puboutput_pub2.publish(msg2);
		ros::spinOnce();
		

		loop_rate.sleep();
*/
		
		pubtrial2::Num msg2;
		//float temp1,temp2, temp3,temp4,temp5,temp6;
		dummyvar var2;
		var2=accelerometerfunc(fd);

		/*temp1=var2.Gx;
		temp2=var2.Gy;
		temp3=var2.Gz;
		temp4=var2.Ax;
		temp4=var2.Ay;
		temp4=var2.Az;
	
		msg2.num1=temp1;
		msg2.num2=temp2;
		msg2.num3=temp3;
		msg2.num4=temp4;
		msg2.num5=temp5;
		msg2.num6=temp6;
*/

		msg2.gyro.x=var2.Gx;
		msg2.gyro.y=var2.Gy;
		msg2.gyro.z=var2.Gz;
		msg2.accel.x=var2.Ax;
		msg2.accel.y=var2.Ay;
		msg2.accel.z=var2.Az;
	    //ROS_INFO("%f", temp1);

		puboutput_pub2.publish(msg2);
		ros::spinOnce();
		

		loop_rate.sleep();


	}

return 0;
}
