#ifndef _LED_H_
#define _LED_H_

#include "common.h"
#include "include.h"

typedef enum{
  led0,
  led1,
  led2,
  led3,
  led4,
  led5,
} LedInstance;

void ledInit();

void ledOn(LedInstance ledInstance);

void ledOff(LedInstance ledInstance);

void ledTurn(LedInstance ledInstance);

#endif  //_LED_H_