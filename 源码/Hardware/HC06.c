#include "stm32f10x.h"                  // Device header

void HC06_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	

	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3, ENABLE);
}

uint8_t Data;
uint8_t DataTemp;
uint8_t Ahead,Back,Left,Right,stop;
uint8_t RxState=0,Serial_RxFlag;
void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		uint8_t UsartTemp = USART_ReceiveData(USART3);


//		if(RxState==0)
//		{
//			if(UsartTemp==0xA5)
//			{
//				RxState=1;
//			}				
//		}
//		else if(RxState==1)
//		{
				Data=UsartTemp;
			
				if     (Data==0x44){Ahead=1;Back=0;Left=0;Right=0;stop=0;}
				else if(Data==0x41){Ahead=0;Back=1;Left=0;Right=0;stop=0;}
				else if(Data==0x4C){Ahead=0;Back=0;Left=1;Right=0;stop=0;}
				else if(Data==0X52){Ahead=0;Back=0;Left=0;Right=1;stop=0;}
				else if(Data==0x4E) {Ahead=0;Back=0;Left=0;Right=0;stop=1;}
				else			 {Ahead=0;Back=0;Left=0;Right=0;stop=1;}
//				RxState=2;
//		}
//		else if (RxState == 2)
//		{
//			if(UsartTemp==0x5A)
//			{
//				RxState=0;
//				Serial_RxFlag=1;
//			}			
//		}
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
