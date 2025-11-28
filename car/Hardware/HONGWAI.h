#ifndef __INFRARED_TRACKING_H
#define __INFRARED_TRACKING_H

#include "stm32f10x.h"

// 全局变量声明
extern uint8_t infrared_left_outer;
extern uint8_t infrared_left_inner;  
extern uint8_t infrared_right_inner;
extern uint8_t infrared_right_outer;
extern int16_t infrared_error;
extern uint8_t infrared_track_state;
extern uint8_t turn_keep_flag;        // 转弯保持标志
extern uint16_t turn_keep_time;       // 转弯保持时间计数
extern uint8_t turn_direction;


// 函数声明
void Infrared_Init(void);
void Infrared_ReadSensors(void);
uint8_t Infrared_GetTrackState(void);
void Infrared_TrackingControl(void);
extern int16_t actualSpeed1;		//定义速度变量
extern int16_t targetSpeed1;
extern int16_t actualSpeed2;		//定义速度变量
extern int16_t targetSpeed2;

#endif

