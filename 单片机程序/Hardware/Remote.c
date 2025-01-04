#include "stm32f10x.h"                  // Device header
#include "Timer.h"

void Remote_Init(uint16_t arr,uint16_t psc)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = arr - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc - 1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_ICInitTypeDef	TIM_ICInitStructure;
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xf;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_CKD_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM3,&TIM_ICInitStructure);
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ITConfig(TIM3,TIM_IT_CC1,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

uint8_t IR_State;
uint16_t IR_Time;
uint8_t IR_RepeatFlag,IR_DataFlag;
uint8_t IR_DATA[4];
uint8_t IR_pData;
uint8_t IR_Address,IR_Command;

uint8_t IR_GetDataFlag()
{
	if (IR_DataFlag)
	{
		IR_DataFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t IR_GetRepeatFlag()
{
	if (IR_RepeatFlag)
	{
		IR_RepeatFlag = 0;
		return 1;
	}
	return 0;
}

uint8_t IR_GetAddress()
{
	return IR_Address;
}

uint8_t IR_GetCommand()
{
	return IR_Command;
}

void TIM3_IRQHandler()
{
	uint8_t Addr_Temp,Cmd_Temp;
	if (TIM_GetITStatus(TIM3,TIM_FLAG_CC1) != RESET)
	{
		if (IR_State == 0)     //空闲态
		{
			TIM_SetCounter(TIM3,0);
			IR_State = 1;
		}
		else if (IR_State == 1) //准备态
		{
			IR_Time = TIM_GetCounter(TIM3);
			if (IR_Time > 13500 - 500 && IR_Time < 13500+500)  //Start信号
			{
				IR_State = 2;
			}
			else if (IR_Time > 11250 - 500 && IR_Time < 11250+500)  //Repeat信号
			{
				IR_State = 0;
				IR_RepeatFlag = 1;
			}
			else
			{
			
			}
			TIM_SetCounter(TIM3,0);
		}
		else if (IR_State == 2) //接收态
		{
			IR_Time = TIM_GetCounter(TIM3);
			if (IR_Time > 1120 -500 && IR_Time < 1120+500)   //逻辑0
			{
				IR_DATA[IR_pData/8] &= ~(0x01 << (IR_pData %8));
				IR_pData++;
			}
			else if (IR_Time > 2250 -500 && IR_Time < 2250 + 500) //逻辑1
			{
				IR_DATA[IR_pData/8] |= (0x01 << (IR_pData %8));
				IR_pData++;
			}
			else 
			{
				IR_pData = 0;
				IR_State = 1;
			}
			
			if (IR_pData>=32)   //如果接收完32位数据
			{
				IR_pData = 0;
				Addr_Temp = ~IR_DATA[1];
				Cmd_Temp  = ~IR_DATA[3];
				if (IR_DATA[0] == Addr_Temp && IR_DATA[2] == Cmd_Temp)  //数据验证
				{
					IR_Address = IR_DATA[0];
					IR_Command = IR_DATA[2];
					IR_DataFlag = 1;
				}
				IR_State = 0;
			}
			TIM_SetCounter(TIM3,0);
		}
		else
		{
		
		}
		TIM_ClearITPendingBit(TIM3,TIM_IT_CC1);
	}
}

