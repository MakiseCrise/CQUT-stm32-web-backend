#include "stm32f10x.h"                  // Device header
#include "HC_SR04.h"
#include "Delay.h"

uint16_t TimeCount;
//	Trig PB15  
//  Echo PC15
void HC_SR04Init()
{
	HC_SR04GPIO_Init();
	HC_SR04TIM_Init();
}

float Sonar()
{
	float Distance,Distance_mm;
	uint32_t Time_end;
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_SET);  //Trig发送触发信号 >10us
	Delay_us(15);
	GPIO_WriteBit(GPIOB,GPIO_Pin_15,Bit_RESET); 
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15) == 0);
	TimeCount = 0;
	while(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15) == 1);
	Time_end = TimeCount*10;
	if (Time_end < 24000)
	{
		Distance = Time_end * 0.000001f * 340 / 2;
		Distance_mm = Distance * 1000;
	}
	return Distance_mm;
}

void HC_SR04GPIO_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	
	//Trig GPIO设置
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_WriteBit(GPIOC,GPIO_Pin_15,Bit_RESET);
}

void HC_SR04TIM_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period  = 10 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3,TIM_FLAG_Update);
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}

void TIM3_IRQHandler()
{
	if (TIM_GetITStatus(TIM3,TIM_IT_Update) == SET)
	{
		TimeCount++;
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
}
