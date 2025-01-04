#include "stm32f10x.h"                  // Device header

void Iwdg_Init()
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	
	IWDG_SetReload(469);
	
	IWDG_ReloadCounter();
	
	IWDG_Enable();
}

void Iwdg_Feed()
{
	IWDG_ReloadCounter();
}

void Wwdg_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	
	WWDG_SetWindowValue(80);
	
	WWDG_Enable(127);
	
	WWDG_ClearFlag();
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =	2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	WWDG_EnableIT();
}

uint8_t WwdgReset_Count;
void WWDG_IRQHandler()
{
	WwdgReset_Count++;
	if (WwdgReset_Count < 10)
	{
		WWDG_SetCounter(127);
	}
	else
	{
		WwdgReset_Count = 10;
	}
	WWDG_ClearFlag();
}
