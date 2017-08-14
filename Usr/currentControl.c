#include "currentControl.h"

int currentControl(int currentTarget){
  static float currentP=35.9;//35.8;//0.8;
  static float currentI=0.6;//0.6;//0.4;//0.4;
  static int I=0;
  int speedDuty=0;
  int current=motorCurrentMeasure();
  
  if(isStart){
    if(currentTarget>1200)
      currentTarget=1200;
    else if(currentTarget<-1200)
      currentTarget=-1200;
    
    float currentErr=currentTarget-current;
    static float lasrCurrentErr=0;
    I+=currentErr*currentI;
    lasrCurrentErr=currentErr;
  
    if(I>8800)
      I=8800;
    else if(I<-8800)
      I=-8800;
    
    speedDuty=I+currentErr*currentP;
    
    if(speedDuty>8800)
      speedDuty=8800;
    else if(speedDuty<-8800)
      speedDuty=-8800;
  }
  
  uartAddWare(currentTarget,7);
  //uartAddWare(current,1);
  
  return speedDuty;
}
