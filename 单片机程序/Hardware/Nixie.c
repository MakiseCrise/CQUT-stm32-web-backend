#include "stm32f10x.h"                  // Device header

uint8_t NixieCode[16] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					    0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};    //数码管键码
void Nixie_Init()
{
	//138译码器引脚初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//数码管LED引脚初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
								  GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|
						 GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
}

void Nixie_Show(uint8_t Location, uint8_t Number)
{
	switch(Location)
	{
		case 1:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 2:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;	
		case 3:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 4:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 5:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 6:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 7:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 8:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		default:break;
	}
	GPIO_WriteLow(GPIOA,NixieCode[Number]);
}

void Nixie_Clean(uint8_t Location)
{
	switch(Location)
	{
		case 1:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 2:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;	
		case 3:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 4:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_SET);
		}break;
		case 5:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 6:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 7:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		case 8:
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);
			GPIO_WriteBit(GPIOB,GPIO_Pin_14,Bit_RESET);
		}break;
		default:break;
	}
	GPIO_WriteLow(GPIOA,0);
}
