#ifndef _KEY_H_
#define _KEY_H_

#include "common.h"
#include "include.h"

typedef enum{
  key0,
  key1,
  key2,
  key3,
  key4,
  key5,
} KeyInstance;

typedef void (*KeyCallback)();

void keyInit();

void keyCallbackInstall(KeyInstance key,KeyCallback keyCallback);

void PORTD_IRQHandler();

void PORTC_IRQHandler();


#endif //_KEY_H_