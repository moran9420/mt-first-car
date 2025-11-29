#include "HONGWAI.h"
#include "key.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

// 全局变量定义
uint8_t infrared_left_outer = 0;
uint8_t infrared_left_inner = 0;  
uint8_t infrared_right_inner = 0;
uint8_t infrared_right_outer = 0;
uint8_t infrared_track_state = 0;
int16_t actualSpeed1=0;		//定义速度变量
int16_t targetSpeed1=0;
int16_t actualSpeed2=0;		//定义速度变量
int16_t targetSpeed2=0;
uint8_t turn_keep_flag = 0;        // 转弯保持标志
uint16_t turn_keep_time = 0;       // 转弯保持时间计数
uint8_t turn_direction = 0; 
int16_t base_speed = 25; // 转弯方向
  
void Infrared_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // 初始化红外传感器引脚 PA2, PA3, PA4, PA5 为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void Infrared_ReadSensors(void)
{	if (turn_keep_flag) {
        return;
    }
  if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) == Bit_RESET) {
        infrared_right_inner = 1;
    } else {
        infrared_right_inner = 0;
    }
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == Bit_RESET) {
        infrared_left_outer = 1;  // 检测到黑线
    } else {
        infrared_left_outer = 0;  // 未检测到黑线
    }
    

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == Bit_RESET) {
        infrared_left_inner = 1;
    } else {
        infrared_left_inner = 0;
    }
    
 

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET) {
        infrared_right_outer = 1;
    } else {
        infrared_right_outer = 0;
    }
}



uint8_t Infrared_GetTrackState(void)
{uint8_t sensor_sum = infrared_left_outer + infrared_left_inner + 
                        infrared_right_inner + infrared_right_outer;
    if(sensor_sum >= 3) {
        return 3; 

   		// 十字路口状态
    } // 计算所有传感器检测到黑线的总数
  
        if (turn_keep_flag) {
        return 7;  // 转弯保持状态，特殊处理
    }
    
    // 十字路口检测：至少3个传感器检测到黑线
   
    // 直线检测：只有中间两个传感器检测到黑线
    else if(infrared_left_inner == 1 && infrared_right_inner == 1 && 
           infrared_left_outer == 0 && infrared_right_outer == 0) {
        return 0;  // 直线状态
    }
    // 左弯检测：左侧传感器检测到黑线，右侧没有
    else if((infrared_left_outer == 1 && infrared_left_inner == 1) && 
            infrared_right_inner == 0 && infrared_right_outer == 0) {
        return 1;  // 左弯状态
    }
    // 右弯检测：右侧传感器检测到黑线，左侧没有
    else if(infrared_right_outer == 1 && infrared_right_inner == 1&& 
            infrared_left_inner == 0 && infrared_left_outer == 0) {
        return 2;  // 右弯状态
    }
	   else if(infrared_left_outer == 1 && infrared_left_inner == 0 && 
            infrared_right_inner == 0 && infrared_right_outer == 0) {
        return 5;  // 左大弯状态
    }
		    else if(infrared_right_outer == 1 && infrared_right_inner == 0 && 
            infrared_left_inner == 0 && infrared_left_outer == 0) {
        return 6;  // 右大弯状态
    }	
				   else if(infrared_left_outer == 0 && infrared_left_inner == 1 && 
            infrared_right_inner == 0 && infrared_right_outer == 0) {
        return 8;  // 左小弯状态
    }
		    else if(infrared_right_outer == 0 && infrared_right_inner == 1 && 
            infrared_left_inner == 0 && infrared_left_outer == 0) {
        return 9;  // 右小弯状态
    }
    else if(sensor_sum == 0) {
        return 4; 
    }
else if(infrared_left_inner == 1 && infrared_right_inner == 1 && 
           infrared_left_outer ==1 && infrared_right_outer == 1) {
        return 10;  // 直线状态
    }
    return 0;
}
void Start_Turn_Keep(uint8_t direction)
{
   turn_keep_flag = 1;
   turn_keep_time =12;  // 设置保持时间
  turn_direction = direction;
}

void Infrared_TrackingControl(void)
{
    static uint8_t last_track_state = 0;  // 保存上一次的循迹状态            // 基础速度值
    if(currentmode==0)
	{targetSpeed1=0;
	targetSpeed2=0;
	return;
	}
    Infrared_ReadSensors();
    

    infrared_track_state = Infrared_GetTrackState();

    if(infrared_track_state == 0) {  
        targetSpeed1 = base_speed;
        targetSpeed2 = base_speed;
    }
    else if(infrared_track_state == 1) {  // 左弯状态
        // 左转弯时，左轮减速，右轮加速
        targetSpeed1 = base_speed - 20;   // 左轮减速
        targetSpeed2 = base_speed + 30;
 		// 右轮加速
    }
    else if(infrared_track_state == 2) {  // 右弯状态
      //   右转弯时，右轮减速，左轮加速
        targetSpeed1= base_speed + 30;   // 左轮加速
        targetSpeed2 = base_speed - 20; 
	 // 右轮减速
	}		
    else if(infrared_track_state == 3) {  
        targetSpeed1= base_speed-10;    // 左轮基准速度
        targetSpeed2 = base_speed-10;   // 右轮基准速度
    }
	    else if(infrared_track_state == 5) {  // 左弯状态
        // 左转弯时，左轮减速，右轮加速
			
        targetSpeed1 = base_speed - 20;   // 左轮减速
        targetSpeed2 = base_speed + 30;
			Start_Turn_Keep(1);
			// 右轮加速
    }
    else if(infrared_track_state == 6) {  // 右弯状态
   
        targetSpeed1= base_speed +30;   // 左轮加速
        targetSpeed2 = base_speed - 20;
		Start_Turn_Keep(2);
		
	}
		    else if(infrared_track_state == 8) {  // 左弯状态
        // 左转弯时，左轮减速，右轮加速
			
        targetSpeed1 = base_speed - 10;   // 左轮减速
        targetSpeed2 = base_speed + 15;  // 右轮加速
    }
    else if(infrared_track_state == 9) {  // 右弯状态
   
        targetSpeed1= base_speed + 15;   // 左轮加速
        targetSpeed2 = base_speed - 10;
		
	}

    else if(infrared_track_state == 4) {  // 丢失路线状态

       if(last_track_state == 1||last_track_state==5) {  // 上一次是左弯

            targetSpeed1 = base_speed - 15;   // 左轮减速
            targetSpeed2 = base_speed + 15;  // 右轮加速
       }
       else if(last_track_state == 2||last_track_state==6) {  // 上一次是右弯

            targetSpeed1 = base_speed +15;   // 左轮加速
            targetSpeed2 = base_speed - 15;  // 右轮减速
        }
        else 
		{
           targetSpeed1 = base_speed -5;   // 左轮低速
            targetSpeed2 = base_speed -5;  // 右轮低速
       }}
    
    
       
     
        
    
	   else if(infrared_track_state == 10) {  // 丢失路线状态

    
       
    
           targetSpeed1 = base_speed-10 ;   // 左轮低速
            targetSpeed2 = base_speed-10; // 右轮低速
        
    }

    else if(infrared_track_state == 7)
	{
	return;
	}
	else 
	{
	  targetSpeed1 = base_speed-5 ;   // 左轮低速
            targetSpeed2 = base_speed-5;
	}

    if(targetSpeed1 > 90) targetSpeed1 =90;
    if(targetSpeed1 < -90) targetSpeed1 = -90;
    if(targetSpeed2 > 90) targetSpeed2 = 90;
    if(targetSpeed2 < -90) targetSpeed2 = -90;
 if (infrared_track_state != 4 && infrared_track_state != 7) {
        last_track_state = infrared_track_state;
    }
           
}

