#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "pid.h"
#include "Timer.h"
#include "PWM.h"
#include "Serial.h"
#include <stdlib.h>
#include "menu.h"
#include "rotate.h"
#include "HONGWAI.h"
int8_t count=0;
uint8_t KeyNum;		//定义用于接收按键键码的变量


uint16_t serialcount=0;
int main(void)
{
	Infrared_Init();/*模块初始化*/
	rotate_Init();
	Serial_Init();
	OLED_Init();		//OLED初始化
	Motor_Init();		//直流电机初始化
	Key_Init();			//按键初始化
	pid_init();
	Timer_Init();
	 Encoder_Init();
	while(1)
	{
		if(flag==1)
	{MENU_Display();
	flag=0;}
	Delay_ms(2);
	
	Key_GetNum();
	menukey();
	int16_t rotatedelta=rotate_Get();
	if(rotatedelta!=0&&currentmode==0)
	{menuencoder(rotatedelta);
	flag=1;}

	Delay_ms(5);
	
}}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{						
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	
		 Infrared_TrackingControl();
		if(currentmode==1)
		{
			actualSpeed2 = Encoder_Get2();
			actualSpeed1 = Encoder_Get1();
			int16_t pwmout=pidspeedcal(targetSpeed1,actualSpeed1);
			int16_t pwmout1=pidspeedcal(targetSpeed2,actualSpeed2);				
			Motor_Setpwm(0,pwmout);
			Motor_Setpwm(1,pwmout1);
		}

	}
}

	
