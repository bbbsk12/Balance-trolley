#include "stm32f10x.h"                  // Device header
/**
 * @brief ��TIM2��ʼ��Ϊ�������ӿ�ģʽ
 * @param psc Ԥ��Ƶϵ��
 * @param arr �Զ���װ��ֵ
 * @retval None
 */
void Encoder_Init_TIM2(void)	
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʹ�ܶ�ʱ��2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    //ʹ��PB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    //�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
    //��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //�����趨������ʼ��GPIOB
    GPIO_Init(GPIOA, &GPIO_InitStructure);

 	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Period = 65535;
    //ѡ��ʱ�ӷ�Ƶ������Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //TIM���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    //ʹ�ñ�����ģʽ3
    TIM_EncoderInterfaceConfig(TIM2, \
                               TIM_EncoderMode_TI12, \
                               TIM_ICPolarity_Rising, \
                               TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);



    //���TIM�ĸ��±�־λ
//    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    //Reset counter
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2, ENABLE);
}

void Encoder_Init_TIM4(void)	
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʹ�ܶ�ʱ��2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    //ʹ��PB�˿�ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    //�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
    //��������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    //�����趨������ʼ��GPIOB
    GPIO_Init(GPIOB, &GPIO_InitStructure);

 	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    // Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Period = 65535;
    //ѡ��ʱ�ӷ�Ƶ������Ƶ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    //TIM���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    //ʹ�ñ�����ģʽ3
    TIM_EncoderInterfaceConfig(TIM4, \
                               TIM_EncoderMode_TI12, \
                               TIM_ICPolarity_Rising, \
                               TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = 10;
    TIM_ICInit(TIM2, &TIM_ICInitStructure);



    //���TIM�ĸ��±�־λ
//    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
 //   TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

    //Reset counter
    TIM_SetCounter(TIM4,0);
    TIM_Cmd(TIM4, ENABLE);
}
/**
 * @brief ��λʱ���ȡ����������
 * @param TIMX ��ʱ��
 * @retval �ٶ�ֵ	�Ǳ��������ص�����
 */
int Read_Encoder2(void)
{
    	int Encoder_TIM;
        Encoder_TIM= (short)TIM2 -> CNT;

        TIM2 -> CNT=0;
   	    return Encoder_TIM;
}

int Read_Encoder4(void)
{
    	int Encoder_TIM;
        Encoder_TIM= (short)TIM4 -> CNT;

        TIM4 -> CNT=0;
   	    return Encoder_TIM;
}
