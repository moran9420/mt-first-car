#ifndef __PID_H
#define __PID_H

typedef struct
{
float kp;
float ki;
float kd;
float jifen;
float lasterror;
float jifenxianzhi;
float outxianzhi;
}piddef;
extern piddef speedpid;
extern piddef positionpid;
void pid_init(void);
int16_t pidcalculate(piddef*pid,float target,float actual);
int16_t pidspeedcal(float targetspeed,float actualspeed);
int16_t pidposcal(float targetpos,float actualpos);
#endif
