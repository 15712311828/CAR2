#ifndef _MENU_H_
#define _MENU_H_

#include "common.h"
#include "include.h"
#include "lcd.h"

typedef void (*itemCallback)();

extern uint8_t singleCarStart;
extern uint8_t doubleCarStart;
extern uint8_t info;
extern uint8_t voltage;

typedef struct{
  uint8_t str[17];
  uint8_t father;
  uint8_t next;
  uint8_t firstChild;
  uint8_t lastChild;
  uint8_t childNum;
  itemCallback callback;
} menuItem;

void editMenuItem();
void menuInit();
uint8_t newMenuItem(uint8_t str[],uint8_t father,itemCallback callback);
void showMenuItem(uint8_t item);
void up();
void down();
void comfirm();
void cancel();
uint8_t choosingItem();
//void PORTC_IRQHandler();
void noClick();







#endif //_MENU_H_