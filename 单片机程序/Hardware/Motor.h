#include "stm32f10x.h"                 

#ifndef _MOTOR_H_
#define _MOTOR_H_

void Motor_Init(void);
void Car_Stop(void);
void Car_Forward(int value);
void Car_Backward(int value);
void Car_TurnLeft(int value);
void Car_TurnRight(int value);
void Car_TransLeft(int value);
void Car_TransRight(int value);
void Motor1_SetSpeed(uint8_t Dir, uint16_t Speed);
void Motor2_SetSpeed(uint8_t Dir, uint16_t Speed);
void Motor3_SetSpeed(uint8_t Dir, uint16_t Speed);
void Motor4_SetSpeed(uint8_t Dir, uint16_t Speed);
void Car_TransForwardRight(int value);
void Car_TransBackwardLeft(int value);
#endif

