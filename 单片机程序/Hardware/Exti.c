#include "stm32f10x.h"
#include "Delay.h"
#include "LED.h"

void Exti_Init()
{
	//BO引脚GPIO初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStrucure; 
	GPIO_InitStrucure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStrucure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStrucure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStrucure);
	
	//BO引脚端口复用为外部中断0
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);

	EXTI_InitTypeDef   EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	//外部中断NVIC
	NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler()
{
	Delay_ms(20);
	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) == 0)
	{
		LED1_Turn();
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}
