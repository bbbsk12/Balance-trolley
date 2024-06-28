#include "stm32f10x.h"                  // Device header

#define PWM_MAX 7200
#define PWM_MIN -7200
void Motor_Init(void)	//IN引脚初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //使能PB端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;	//端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
    GPIO_Init(GPIOB, &GPIO_InitStructure);				 //根据设定参数初始化GPIOB
}

void Motor_PWM_Init(void)
{
	/******************************开启时钟*****************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//开启定时器1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//开启GPIO时钟
	/*****************************定义结构体变量******************************/	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/****************************配置引脚的模式*******************************/	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************配置时基单元*******************************/	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1/2/4分频
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//向上计数
	TIM_TimeBaseInitStructure.TIM_Period =7199;		//ARR：自动重装载寄存器的值 Freq = CK_PSC/(PSC+1)/(ARR+1) 100-1
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0 ;	//PSC：预分频系数，72MHz，那么可以给PSC=0，36MHz计数时，那么就给PSC=1    36-1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//TIM_RepetitionCounter：重复计数，也就是重复多少次才发生计数溢出中断。官方注释里面这个数只与TIM1和TIM8有关
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	/******************************输出比较单元配置*****************************/		
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//比较输出模式选择
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//比较输出极性，可选为高电平有效或低电平有效
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //比较输出脉冲宽度，实际设定比较寄存器 CCR 的值，决定脉冲宽度。可设置范围为 0 至 65535。PWM占空比：Duty = CCR / (ARR + 1) = CCR / 100
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//结构体初始化    
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//结构体初始化
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能 TIMx 在 CCRx 上的预装载寄存器
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//使能或者失能 TIMx 在 CCRx 上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);//开启定时器


}


void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM1, Compare);
}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM1, Compare);
}
int abs(int p)
{
	if(p>0)return p;
	else   return -p;
}
	
void Load(int Motor1,int Motor2)
{
	if(Motor1<0)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)0); 
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)1);
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,(BitAction)1); 
		GPIO_WriteBit(GPIOB,GPIO_Pin_13,(BitAction)0);

	}
	TIM_SetCompare4(TIM1, abs(Motor1));

	if(Motor2<0)
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)0); 
		GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)1);
	}
	else
	{
		GPIO_WriteBit(GPIOB,GPIO_Pin_14,(BitAction)1); 
		GPIO_WriteBit(GPIOB,GPIO_Pin_15,(BitAction)0);

	}
	TIM_SetCompare1(TIM1, abs(Motor2));
}

void Limit(int* MOTO1,int* MOTO2)
{
	if(*MOTO1>PWM_MAX)  *MOTO1=PWM_MAX;
	if(*MOTO1<PWM_MIN)  *MOTO1=PWM_MIN;	
	if(*MOTO2>PWM_MAX)  *MOTO2=PWM_MAX;
	if(*MOTO2<PWM_MIN)  *MOTO2=PWM_MIN;	
}
