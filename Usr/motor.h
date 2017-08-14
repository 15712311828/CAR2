#ifndef _MOTOR_H_
#define _MOTOR_H_


#include "common.h"
#include "include.h"

void motorInit();

void changeSpeedDuty(int duty);

void changeDirDuty(int duty);

void motorDuty(int duty1,int duty2);

void motorStart();

void motorStop();






#endif  //_MOTOR_H_