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
int8_t count=0;
uint8_t flag=1;
uint8_t KeyNum;		//定义用于接收按键键码的变量
int16_t actualSpeed1=0;		//定义速度变量
int16_t targetSpeed1=0;
int16_t actualSpeed2=0;		//定义速度变量
int16_t targetSpeed2=0;
int32_t actualpos1=0;		//定义速度变量
int32_t targetpos1=0;
int32_t actualpos2=0;		//定义速度变量
int32_t targetpos2=0;
uint16_t serialcount=0;
int main(void)
{
	/*模块初始化*/
	Serial_Init();
	OLED_Init();		//OLED初始化
	Motor_Init();		//直流电机初始化
	Key_Init();			//按键初始化
	pid_init();
	Timer_Init();
	 Encoder_Init();
	while(1)
	{
	Key_GetNum();
			if(Serial_RxFlag==1)
		{
		targetSpeed1=atoi(Serial_RxPacketnum);
			Serial_RxFlag=0;
		}
	if(currentmode==0)
	{
	OLED_ShowString(1,1,"speedmode");
	}
	else
	{
			OLED_ShowString(1,1,"positionmode");
	}
	Delay_ms(10);
	}
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{		static int16_t lastpos=0;					//每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);	
		serialcount++;
		if(currentmode==0)
		{
			
			actualSpeed1 = Encoder_Get1();
			int16_t pwmout=pidspeedcal(targetSpeed1,actualSpeed1);			
			Motor_Setpwm(0,pwmout);
			if(serialcount>=5)
			{
			serialcount=0;
			usart_sendspeed(targetSpeed1,actualSpeed1);
			}
		}

	}
}
