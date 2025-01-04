#include "stm32f10x.h"                  // Device header
#include "Usart.h"

#pragma import(__use_no_semihosting)//关闭ARM的半主机模式

#include <stdio.h>

struct __FILE
{
	int handle; /* Add whatever you need here */ 
};

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE *f) 
{
	Usart1_SendByte(ch);
	return ch;
}

void _sys_exit(int x) 
{
	x = x; /* endless loop */
}

char Rx_Data;
uint8_t Rx_Flag;

uint8_t RxState;
uint8_t pRxPacket;
char mode;
int sendFlag = 0;
char Rx_Packet[100];
int waveDistance = 20;

void changeWaveDistance(int value){
	waveDistance = value;
}
int getWaveDistance(){
	return waveDistance;
}

void Usart1_Init(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//tx
	GPIO_InitTypeDef	GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	USART_InitTypeDef	USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_InitTypeDef	NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}

void Usart1_SendByte(uint8_t Byte){
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Usart1_SendArray(uint8_t *Array,uint16_t Length){
	uint16_t i;
	for (i = 0; i < Length; i++)
	{
		Usart1_SendByte(Array[i]);
	}
}

void Usaert1_SendPacket(uint8_t *Array,uint16_t Length){
	Usart1_SendByte(0xFF);
	Usart1_SendArray(Array,Length);
	Usart1_SendByte(0xFE);
}

void Usart1_SendString(char *str){
	while (*str) {
        USART_SendData(USART1, (uint8_t)*str);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); // 等待发送完成
        str++;
    }
//	uint8_t i;
//	for(i = 0; String[i] != '\0';i++)
//	{
//		Usart1_SendByte(String[i]);
//	}
}

//进值转换
uint32_t Usart1_Pow(uint32_t X, uint32_t Y){
	uint32_t Result = 1;
	while (Y--)
	{
		Result *= X;
	}
	return Result;
}

void Usart1_SendNum(uint32_t Number, uint8_t Length){
	uint8_t i;
	for (i = 0; i < Length; i++)							
	{
		Usart1_SendByte(Number / Usart1_Pow(10, Length - i - 1) % 10 + '0');
	}
}

uint8_t Usart1_GetRxFlag(){
	if (Rx_Flag == 1)
	{
		Rx_Flag = 0;
		return 1;
	}
	return 0;
}

uint8_t Usart1_GetRxData(){
	return Rx_Data;
}

void USART1_IRQHandler(){
	if (USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET)
	{
		Rx_Data = USART_ReceiveData(USART1);
		if (RxState == 0)
		{
			if (Rx_Data == '@')
			{
				RxState = 1;
				pRxPacket = 0;
			}else if(Rx_Data == '!'){
				RxState = 3;
			}else if(Rx_Data == '#'){
				pRxPacket = 0;
				RxState = 4;
			}
		}
		else if (RxState == 1)
		{
			if (Rx_Data == '\r')
			{
				RxState = 2;
			}
			else
			{
				Rx_Packet[pRxPacket] = Rx_Data;
				pRxPacket++;			
			}
		}
		else if (RxState == 2)
		{
			if (Rx_Data== '\n')
			{
				RxState = 0;
				Rx_Packet[pRxPacket] = '\0';
				Rx_Flag = 1;
			}
		}
		else if(RxState == 3){
			if (Rx_Data == '\r')
			{
				RxState = 2;
			}
			else if (Rx_Data == '@')
			{
				sendFlag = 1;
			}
			else
			{
				mode = Rx_Data;		
			}
		}else if(RxState == 4){
			if (Rx_Data == '\r')
			{
				RxState = 2;
				Rx_Packet[pRxPacket] = '\0';
				waveDistance = StringToInt(Rx_Packet);
			}
			else
			{
				Rx_Packet[pRxPacket] = Rx_Data;
				pRxPacket++;			
			}
		}
		
		USART_ClearITPendingBit(USART1,USART_FLAG_RXNE);
	}
}

int StringToInt(char* str) {
    int num = 0;
    int i = 0;
    while (str[i] != '\0') {
        num = num * 10 + (str[i] - '0');
        i++;
    }
    return num;
}

int getSendFlag(){
	return sendFlag;
}
void setSendFlag(int value){
	sendFlag = value;
}

char getMod(){
	if(mode){
		return mode;
	}
	return '0';
}
