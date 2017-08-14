#ifndef _SPEED_CONTROL_H_
#define _SPEED_CONTROL_H_

#include "common.h"
#include "include.h"

extern int speed;
extern int speedLever;

int speedControl(int speedTarget);

int speedTarget();


#endif //_SPEED_CONTROL_H_