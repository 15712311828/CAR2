#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "common.h"
#include "include.h"

extern int interruptTime;
extern int startTime;
extern int isStart;
extern int isRun;
extern int stopMode;

typedef void (*pInterrupt)();
extern int interruptTime;
void systemInit();
void systemInterruptCallbackInstall(pInterrupt pf);
void systemInterruptOn();
extern pInterrupt pfunc_handler;
void systemInterrupt();
void exceptionStop();

void ring();

void startRun();


#endif //_SYSTEM_H_