#ifndef __ROTATE_H
#define __ROTATE_H
extern uint16_t flag;
void rotate_Init(void);
void EXTI3_IRQHandler(void);
int16_t rotate_Get(void);
void EXTI4_IRQHandler(void);
#endif
