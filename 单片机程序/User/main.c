#include "stm32f10x.h"                  
#include "Delay.h"
#include "Motor.h"
#include "OLED.h"
#include "Usart.h"
#include "LED.h"
#include "String.h"
#include "PS2.h"
#include "Sensor.h"
#include "HC_SR04.h"
#include "cJSON.h"


void setMod(int mode);
void workModController(char mode);
void remoteControlMod();
void leverControlMod();
void autoFollowOnlineMod();
void autoFollowOnwaveMod();
void nvic_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority);

uint8_t KeyNum;
uint8_t PS2_Mode,Last_PS2_Mode;

uint8_t SensorState[4];
uint8_t Area,Last_Area;
uint8_t TxData[10];
int TXFlag = 0;
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Usart1_Init();
	LED_Init();
	Motor_Init();
	Sensor_Init();
	HC_SR04Init();
	PS2_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	TIM_TimeBaseInitTypeDef TIM_Initer;
	TIM_Initer.TIM_ClockDivision=TIM_CKD_DIV1;  
	TIM_Initer.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_Initer.TIM_Period = 10000-1;  //配置频率参数
	TIM_Initer.TIM_Prescaler = 7200-1;  //配置频率参数
	TIM_Initer.TIM_RepetitionCounter = 0;  //配置计数器，此处用不到
	TIM_TimeBaseInit(TIM1,&TIM_Initer);
	
	TIM_ClearFlag(TIM1,TIM_FLAG_Update);  //初始化清理一次定时器flag，防止初始化时自动计时一次
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	TIM_Cmd(TIM1,ENABLE);
	nvic_Init(TIM1_UP_IRQn,0,0);	

	
	while(1)
	{
		
		workModController(getMod());
		
		if(getSendFlag() == 1){
					TxData[0] = getMod() - '0';
		
					TxData[1] = Sensor0_Get_State();
					TxData[2] = Sensor1_Get_State();
					TxData[3] = Sensor2_Get_State();
					TxData[4] = Sensor3_Get_State();
					
					TxData[5] = Sonar();
					
					TxData[6] = TIM_GetCapture1(TIM2)+TIM_GetCapture2(TIM2);
					TxData[7] = TIM_GetCapture4(TIM2)+TIM_GetCapture3(TIM2);
					TxData[8] = TIM_GetCapture2(TIM4)+TIM_GetCapture1(TIM4);
					TxData[9] = TIM_GetCapture3(TIM4)+TIM_GetCapture4(TIM4);
			
					Usart1_SendArray(TxData,10);
					setSendFlag(0);
					Delay_ms(10);
				
				
		}
		
	}
}

void workModController(char mode){
	switch(mode){
		case '0': leverControlMod(); break;
		case '1':	remoteControlMod(); break;
		case '2': autoFollowOnlineMod(); break;
		case '3': autoFollowOnwaveMod(); break;
	}
}

void remoteControlMod(){
	if (Usart1_GetRxFlag())
		{
			if (strcmp(Rx_Packet,"W") == 0)
			{
				Car_Forward(1000);
			}
			else if (strcmp(Rx_Packet,"S") == 0)
			{
				Car_Backward(1000);
			}
			else if (strcmp(Rx_Packet,"Q") == 0)
			{
				Car_TurnLeft(1000);
			}
			else if (strcmp(Rx_Packet,"E") == 0)
			{
				Car_TurnRight(1000);
			}
			else if (strcmp(Rx_Packet,"A") == 0)
			{
				Car_TransLeft(1000);
			}
			else if (strcmp(Rx_Packet,"D") == 0)
			{
				Car_TransRight(1000);
			}
			else
			{
				Car_Stop();
			}
		}
}

void leverControlMod(){
		KeyNum = ps2_key_serch();
		PS2_Mode = ps2_mode_get();
		
		if (PS2_Mode != Last_PS2_Mode)
		{
			Car_Stop();
		}
		
		if (PS2_Mode == PSB_REDLIGHT_MODE)
		{
			unsigned char ps2_lx,ps2_ly;
			
			ps2_lx = ps2_get_anolog_data(PSS_LX);
			ps2_ly = ps2_get_anolog_data(PSS_LY);
			
			if ( ps2_ly == 0x00)
			{
				Car_Forward(1000);
			}
			else if (ps2_ly == 0xff)
			{
				Car_Backward(1000);
			}
			else if (ps2_lx == 0x00)
			{
				Car_TransLeft(1000);
			}
			else if (ps2_lx == 0xff)
			{
				Car_TransRight(1000);
			}
			else
			{
				if (ps2_get_key_state(PSB_L1))
				{
					Car_TurnLeft(1000);
				}
				else if (ps2_get_key_state(PSB_L2))
				{
					Car_TurnRight(1000);
				}				
				else
				{
					Car_Stop();
				}			
			}
		}	
		else if (PS2_Mode == PSB_GREENLIGHT_MODE)
		{
			if (KeyNum)
			{
				if (ps2_get_key_state(PSB_PAD_UP))
				{
					Car_Forward(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_DOWN))
				{
					Car_Backward(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_LEFT))
				{
					Car_TransLeft(1000);
				}
				else if (ps2_get_key_state(PSB_PAD_RIGHT))
				{
					Car_TransRight(1000);
				}	
				else if (ps2_get_key_state(PSB_L1))
				{
					Car_TurnLeft(1000);
				}
				else if (ps2_get_key_state(PSB_L2))
				{
					Car_TurnRight(1000);
				}				
				else
				{
					Car_Stop();
				}
			}
			else
			{
				Car_Stop();
			}
		}
		else
		{
			
		}
		Last_PS2_Mode = PS2_Mode;
		Delay_ms(10);
}

void autoFollowOnlineMod(){
		SensorState[0]=Sensor0_Get_State();
		SensorState[1]=Sensor1_Get_State();
		SensorState[2]=Sensor2_Get_State();
		SensorState[3]=Sensor3_Get_State();
		
		if (SensorState[0] && !SensorState[1] && !SensorState[2] && SensorState[3])   //Area 3，当前处于直线
		{
			Area = 3;
			Car_Forward(200);
		}
		else if (SensorState[0] && SensorState[1] && !SensorState[2] && SensorState[3]) //Area4, 右小弯道
		{
			Area = 4;
			Car_TurnRight(200);
		}
		else if (SensorState[0] && SensorState[1] && SensorState[2] && !SensorState[3]) //Area6 ,右大弯
		{
			Area = 6;
			Car_TurnRight(300);
		}
		else if (SensorState[0] && !SensorState[1] && SensorState[2] && SensorState[3]) //Area2 ,左小弯
		{
			Area = 2;
			Car_TurnLeft(200);
		}
		else if (!SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3]) //Area0 ,左大弯
		{
			Area = 0;
			Car_TurnLeft(300);
		}				
		else if (SensorState[0] && SensorState[1] && SensorState[2] && SensorState[3])
		{
			if (Last_Area == 4)   //右中等弯
			{
				Area = 5;
				Car_TurnRight(250);
			}
			else if (Last_Area == 2)  //左中等弯
			{
				Area = 1;
				Car_TurnLeft(250);
			}
		}
		Last_Area = Area;
//		Delay_ms(30);
}

void autoFollowOnwaveMod(){
		int Distance_mm = Sonar();
		int Distance_cm = Distance_mm / 10;
		int Distance_cm_p = Distance_mm % 10;
				
		if (Distance_cm ==0 && Distance_cm_p==0)
		{
			Car_Stop();
		}
		else
		{
			if (Distance_cm > getWaveDistance())
			{
				Car_Forward(1000);
			}
			else if (Distance_cm < ((getWaveDistance()-10)>0 ? (getWaveDistance()-10):5) )
			{
				Car_Backward(1000);
			}
			else
			{
				Car_Stop();
			}
		}
		Delay_ms(100);
}

void TIM1_UP_IRQHandler(){
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == SET){
		TXFlag = 1;
		//约定：0 0000 "0" 0000
		//     工作模式 红外巡线 超声波测距 电机工作
//	Usart1_SendByte(getMod());
//		TxData[0] = getMod() - '0';
//		
//		TxData[1] = Sensor0_Get_State();
//		TxData[2] = Sensor1_Get_State();
//		TxData[3] = Sensor2_Get_State();
//		TxData[4] = Sensor3_Get_State();
//		
//		TxData[5] = Sonar();
//		
//		TxData[6] = TIM_GetCapture1(TIM2);
//		TxData[7] = TIM_GetCapture2(TIM2);
//		TxData[8] = TIM_GetCapture1(TIM4);
//		TxData[9] = TIM_GetCapture2(TIM4);
//		//////
//		Usart1_SendArray(TxData,10);
//		Delay_ms(10);
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
	}
}

void nvic_Init(uint8_t NVIC_IRQChannel,uint8_t NVIC_IRQChannelPreemptionPriority,uint8_t NVIC_IRQChannelSubPriority){
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = NVIC_IRQChannel;  //EXTI0_IRQn
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = NVIC_IRQChannelPreemptionPriority;  //0
	nvic.NVIC_IRQChannelSubPriority = NVIC_IRQChannelSubPriority;  //0
	NVIC_Init(&nvic);
}
