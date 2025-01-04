#include "stm32f10x.h"                  // Device header
#include "MatrixKey.h"
#include "Delay.h"

void MatrixKey_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_WriteLow(GPIOB,0xff);
//	GPIO_SetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_3);
}

uint8_t MatrixKey_Scan()
{
	uint8_t KeyNum = 0,RowNum =0;
	
	GPIO_WriteLow(GPIOB,0xff);
	GPIO_WriteBit(GPIOB, GPIO_Pin_3,Bit_RESET);   //将第一列输出1，此时扫描行。若行返回值不为0，则有按键按下。
	RowNum = MatrixKey_Row_Scan();
	if (RowNum != 0)
	{
		KeyNum = 1 + (RowNum - 1) * 3;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_3,Bit_SET);
	
//	GPIO_WriteLow(GPIOB,0xff);
	GPIO_WriteBit(GPIOB, GPIO_Pin_1,Bit_RESET);   //将第三列输出1，此时扫描行。若行返回值不为0，则有按键按下。
	RowNum = MatrixKey_Row_Scan();
	if (RowNum != 0)
	{
		KeyNum = 3 + (RowNum - 1) * 3;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_1,Bit_SET);
	
//	GPIO_WriteLow(GPIOB,0xff);
	GPIO_WriteBit(GPIOB, GPIO_Pin_0,Bit_RESET);  //将第四列输出1，此时扫描行。若行返回值不为0，则有按键按下。
	RowNum = MatrixKey_Row_Scan();
	if (RowNum != 0)
	{
		KeyNum = 4 + (RowNum - 1) * 3;
	}
	GPIO_WriteBit(GPIOB, GPIO_Pin_0,Bit_SET);
	
	return KeyNum;
}

uint8_t MatrixKey_Row_Scan()
{
	uint8_t KeyRow = 0;
	uint8_t Row1,Row2,Row3,Row4;
//	if ( == Bit_RESET)
//	{
//		Delay_ms(10);
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == Bit_RESET);
//		{
//			KeyRow = 1;
//			return KeyRow;
//		}
//	}
	Row1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
	if ( Row1 == Bit_RESET)
	{
		Delay_ms(10);
		Row1 = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
		if(Row1 == Bit_RESET);
		{
			KeyRow = 1;
			return KeyRow;
		}
	}	
	
//	if ((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == Bit_RESET))
//	{
//		Delay_ms(10);
//		if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6) == Bit_RESET))
//		{
//			KeyRow = 2;
//			return KeyRow;
//		}
//	}
//	
//	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == Bit_RESET)
//	{
//		Delay_ms(10);
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5) == Bit_RESET)
//		{
//			KeyRow = 3;
//			return KeyRow;
//		}
//	}
//	
//	if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == Bit_RESET)
//	{
//		Delay_ms(10);
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4) == Bit_RESET)
//		{
//			KeyRow = 4;
//			return KeyRow;
//		}
//	}
	
	KeyRow = 0;
	return KeyRow;
}
