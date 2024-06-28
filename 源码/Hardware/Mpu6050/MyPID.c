#include "stm32f10x.h"      // Device header
#include "encoder.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "MPU6050.h"
#include "Motor.h"
#include "HC06.h"
extern uint8_t stop;
#define VKP 0.45
//*PID调参*//
float Vertical_Kp=-300,Vertical_Kd=-1.22;//直立环负极 （-360 -1.08） （-380 -1.5）
float Verlocity_Kp= VKP,Verlocity_Ki= VKP/200;//速度环正极 （0.36 0.0018） （0.42）
float Turn_Kp=-2,Turn_Kd=0;//转向环 （-10 -0.36）要顺利转向得给点前进速度 200
int Encoder_S_Limit=2000;
int Encoder_L,Encoder_R;
跟中值一样，翻滚角，偏航角
short ax,ay,az,gx,gy,gz;	
short a[3],g[3];
int Vertical_Out,Verlocity_Out,Turn_Out,MOTO1,MOTO2;
short Target_Speed,Target_Turn;

int SPEED_MAX=40;
int TURN_MAX=350;
extern uint8_t Ahead,Back,Left,Right;
//直立环控制PD 期望角度、真实角度、角速度
int Vertical(float Med,float Angle,float gyro_Y)
{
	int temp;
	temp=Vertical_Kp*(Angle-Med)+Vertical_Kd*gyro_Y;
	return temp;
}

//速度控制环PI 期望速度、左编码器、右编码器
int Verlocity(float Target,int Motor_L,int Motor_R)
{
	
	static int Err_LowOut_last,Encoder_S;
	static float a=0.7;
	int Err,Err_LowOut,temp;
	Verlocity_Ki=Verlocity_Kp/200;
	
	Err=(Motor_L+Motor_R)- Target;
	Err_LowOut=(1-a)*Err+a*Err_LowOut_last;
	Err_LowOut_last=Err_LowOut;
	
	Encoder_S+=Err_LowOut;

	Encoder_S=Encoder_S>Encoder_S_Limit?Encoder_S_Limit:(Encoder_S<-Encoder_S_Limit?-Encoder_S_Limit:Encoder_S);
	if(stop==1)Encoder_S=0,stop=0;
	
	temp=Verlocity_Kp*Err_LowOut+Verlocity_Ki*Encoder_S;
	return temp;
}

//转向控制环PD 角速度、角度值
int Turn(float gyro_Z,int Targe)
{
	int temp;
	temp=Turn_Kp*Targe+Turn_Kd*gyro_Z;
	return temp;
}
void Control(void)
{
	int PWM_Out=0;
	Encoder_L= Read_Encoder2();
	Encoder_R=-Read_Encoder4();
	//1.读取传感器数值 编码器 陀螺仪
	MPU6050_DMP_Get_Data(&Pitch,&Roll,&Yaw,g,NULL);
	MPU_Get_Accelerometer(&ax,&ay,&az);
	MPU_Get_Gyroscope(&gx,&gy,&gz);	
	
	if(Ahead==0 && Back==0)Target_Speed=0;
	if(Ahead==1)Target_Speed--;
	if(Back==1)Target_Speed++;
	Target_Speed=Target_Speed>SPEED_MAX?SPEED_MAX:(Target_Speed<-SPEED_MAX?-SPEED_MAX:Target_Speed);
	
	if(Left==0 && Right==0){Target_Turn=0;}
	if(Left==1){Target_Turn+=30;Target_Speed=20;}
	if(Right==1){Target_Turn-=30;Target_Speed=20;}
	Target_Turn=Target_Turn>TURN_MAX?TURN_MAX:(Target_Turn<-TURN_MAX?-TURN_MAX:Target_Turn);
	
	if(Left==0 && Right==0)Turn_Kd=0;
	else if(Left==1 || Right==1)Turn_Kd=0;
	
	Verlocity_Out=Verlocity(Target_Speed,Encoder_L,Encoder_R);
	Vertical_Out=Vertical(Verlocity_Out-Med_Angle,Roll,g[0]);
	Turn_Out=Turn(g[2],Target_Turn);
	
	PWM_Out=Vertical_Out;
	
	MOTO1=PWM_Out-Turn_Out;
	MOTO2=PWM_Out+Turn_Out;
	Limit(&MOTO1,&MOTO2);
	Load(MOTO1,MOTO2);
}
