#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_Init(void);
int16_t Encoder_Get1(void);
int16_t Encoder_Get2(void);
extern int16_t posencoder1;
extern int16_t posencoder2;

#endif
