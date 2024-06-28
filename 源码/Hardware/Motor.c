#include "stm32f10x.h"                  // Device header

#define PWM_MAX 7200
#define PWM_MIN -7200
void Motor_Init(void)	//IN���ų�ʼ��
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //ʹ��PB�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 |GPIO_Pin_13 | GPIO_Pin_14|GPIO_Pin_15;	//�˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;      //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
    GPIO_Init(GPIOB, &GPIO_InitStructure);				 //�����趨������ʼ��GPIOB
}

void Motor_PWM_Init(void)
{
	/******************************����ʱ��*****************************/	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);	//������ʱ��1ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//����GPIOʱ��
	/*****************************����ṹ�����******************************/	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	/****************************�������ŵ�ģʽ*******************************/	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/****************************����ʱ����Ԫ*******************************/	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//1/2/4��Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period =7199;		//ARR���Զ���װ�ؼĴ�����ֵ Freq = CK_PSC/(PSC+1)/(ARR+1) 100-1
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0 ;	//PSC��Ԥ��Ƶϵ����72MHz����ô���Ը�PSC=0��36MHz����ʱ����ô�͸�PSC=1    36-1
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;//TIM_RepetitionCounter���ظ�������Ҳ�����ظ����ٴβŷ�����������жϡ��ٷ�ע�����������ֻ��TIM1��TIM8�й�
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	/******************************����Ƚϵ�Ԫ����*****************************/		
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//�Ƚ����ģʽѡ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//�Ƚ�������ԣ���ѡΪ�ߵ�ƽ��Ч��͵�ƽ��Ч
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //�Ƚ���������ȣ�ʵ���趨�ȽϼĴ��� CCR ��ֵ�����������ȡ������÷�ΧΪ 0 �� 65535��PWMռ�ձȣ�Duty = CCR / (ARR + 1) = CCR / 100
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);//�ṹ���ʼ��    
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//�ṹ���ʼ��
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ�� TIMx �� CCRx �ϵ�Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);//ʹ�ܻ���ʧ�� TIMx �� CCRx �ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);//������ʱ��


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
