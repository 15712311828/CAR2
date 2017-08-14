#include "system.h"

int interruptTime;
int startTime;
//起跑位
int isRun=0;
//直立位
int isStart=0;
//系统初始化

int stopMode=0;
void systemInit(){
  pit_init_ms(PIT0, 5);
  set_vector_handler(PIT0_VECTORn ,systemInterrupt);
  pfunc_handler=NULL;
  NVIC_SetPriority(PIT0_IRQn, 0xF);
  gpio_init(PTC6,GPO,0);
  interruptTime=0;
  startTime=0;
}

void systemInterruptCallbackInstall(pInterrupt pf){
  pfunc_handler=pf;
}

void systemInterruptOn(){
  enable_irq (PIT0_IRQn); 
}

pInterrupt pfunc_handler;

int ringTick=0;
void systemInterrupt(){
  if(pfunc_handler){
    pfunc_handler();
    interruptTime++;
    if(isStart){
      startTime++;
      if(startTime==500){
        isRun=1;
      }
    }
  }
  if(ringTick){
    ringTick--;
    if(ringTick==0)
      gpio_set(PTC6,0);
  }
  PIT_Flag_Clear(PIT0); 
}

void exceptionStop(){
  disable_irq(PIT0_IRQn);
  while(1){
    motorDuty(0,0);
    DELAY_MS(500);
    ledOn(led0);
    ledOn(led1);
    ledOn(led2);
    ledOn(led3);
    DELAY_MS(500);
    ledOff(led0);
    ledOff(led1);
    ledOff(led2);
    ledOff(led3);
  }
}

void ring(){
  ringTick=5;
  gpio_set(PTC6,1);
}

void startRun(){
  isStart=1;
  ledOn(led0);
}
