	#include "stm32f10x.h"                 
	#include "OLED.h"
	#include "LED.h"
	#include "menu.h"
	#include "Key.h"
	#include "Encoder.h"
	#include "pid.h"
	uint16_t single[4]={0};
	uint16_t currentmenu=0;
	uint16_t mode=0;
	uint16_t biao=0;
		void menuencoder(int16_t encoderdelta)
	{

		if(currentmenu ==2 && encoderdelta!=0) {
			float step = 0.1f;
			
			if(biao == 0) {
				speedpid.kp = speedpid.kp + encoderdelta * step;
				if(speedpid.kp < 0) {
					speedpid.kp = 0;
				}
			} else if(biao == 1) {	
				speedpid.ki = speedpid.ki + encoderdelta * step;
				if(speedpid.ki < 0) {
					speedpid.ki = 0;
				}
			} else if(biao == 2) {	
				speedpid.kd = speedpid.kd + encoderdelta * step;
				if(speedpid.kd < 0) {
					speedpid.kd = 0;
				}
			}
		}
	}void menukey(void)
	{if(single[1]==1)
	{
	if(biao==0)
	biao=2;
	else biao=biao-1;
		 single[1] = 0;}
	else if(single[2]==1)
	{
	biao=(biao+1)%3;}
 single[2] = 0;
	}
void MENU_Display(void)
{
			OLED_ShowString(1,1, "car mode");
			OLED_ShowString(2, 2, "kp");
			OLED_ShowString(3, 2, "ki");
			OLED_ShowString(4, 2, "kd");
		 char b[20],c[20],d[20];
		 sprintf(b,"%.1f",speedpid.kp);
		 sprintf(c,"%.1f",speedpid.ki);
		 sprintf(d,"%.1f",speedpid.kd);
			OLED_ShowString(2, 14,b);
			OLED_ShowString(3,14,c);
			OLED_ShowString(4,14,d);
	   if(biao == 0) {
				OLED_ShowString(2, 1, ">");
			} else if(biao == 1) {
				OLED_ShowString(3, 1, ">");}
			else if(biao==2)
			{OLED_ShowString(4, 1, ">");}}