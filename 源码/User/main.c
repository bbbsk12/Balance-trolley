#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "inv_mpu.h"
#include "Motor.h"
#include "encoder.h"
#include "MyPID.h"
#include <stdio.h>
#include "HC06.h"
char Temp[16];
extern float Pitch,Roll,Yaw;							
extern short a[3],g[3];								
extern short ax,ay,az,gx,gy,gz;
extern int Encoder_L,Encoder_R;
extern short Target_Speed,Target_Turn;
extern uint8_t Data;;

int main(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	OLED_Init();
	MPU6050_Init();
	MPU6050_DMP_Init();
	MPU6050_INT_Init();
	Motor_Init();	      //INÒý½Å³õÊ¼»¯
	Motor_PWM_Init();
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	HC06_Init();
	while (1)
	{

		sprintf(Temp,"Encoder_L:%d    ",Encoder_L);
		OLED_ShowString(1,1,Temp);
		sprintf(Temp,"Encoder_R:%d    ",Encoder_R);
		OLED_ShowString(2,1,Temp);
		sprintf(Temp,"Roll:%.1f    ",Roll);
		OLED_ShowString(3,1,Temp);
		sprintf(Temp,"%.0f,%.0f    ",(double)gx,(double)gz);
		OLED_ShowString(4,1,Temp);

		
	
		
	}
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) == SET)
	{


		Control();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
}
