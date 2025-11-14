#ifndef __MENU_H
#define __MENU_H

void menuinit(void);
void menukey(void);
void menuencoder(int16_t encoderdelta);
void MENU_Display(void);
extern uint16_t mode;
extern uint16_t currentmenu;
#endif


