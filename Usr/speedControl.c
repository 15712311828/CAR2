#include "speedControl.h"


int speed;
//µç»ú±Õ»·
int speedControl(int speedTarget){
  static float speedPControl[9]={200,100,60,40,0,-20,-60,-180,-290};//;{300,200,100,40,0,-10,-30,-120,-240};//{200,100,30,7,0,-7,-30,-100,-200};
  static float speed_s[9]={-800,-400,-200,-100,0,100,200,400,800};
  static float speedI=0.13;//0.015;//0.012;//0.006//0.046;//0.05;//0.0188;//0.05;//20;
  static int control=150;
  static int oldControl=150;
  
  if(interruptTime%20==0)
    speed=encoderMeasure();

  
  if(isStart&&interruptTime%20==0){
    oldControl=control;
    static float angleOffset=150;
    float speedErr=speedTarget-speed;
    static float lasrSpeedErr=0;
    static float lastSpeed=0;
    static float I=150;
    static int IQueue[200]={0};
    static int IPos=0;
    float PControl=0;
    if(speedErr<=speed_s[0])
      PControl=speedPControl[0];
    else if(speedErr>=speed_s[8])
      PControl=speedPControl[8];
    else{
      for(int i=0;i<8;i++){
        if(speedErr>=speed_s[i]&&speedErr<=speed_s[i+1]){
          PControl=speedPControl[i]+(speedPControl[i+1]-speedPControl[i])*(speedErr-speed_s[i])/(speed_s[i+1]-speed_s[i]);
        }
      }
    }
    //PControl=speedErr*(-0.5);
    //I+=IQueue[IPos]*speedI;
    if(fabs(speedErr)<400)
      I-=speedErr*speedI;
    else if(speedErr>=200)
      I-=200*speedI;
    else if(speedErr<=-200)
      I-=-200*speedI;
    IQueue[IPos]=speedErr;
    IPos++;
    if(IPos>=200){
      IPos=0;
    }
    /*if(I>170)
      I=170;
    else if(I<50)
      I=50;*/
    //lasrSpeedErr=speedErr;
    
    if(I>100+angleOffset)
      I=100+angleOffset;
    else if(I<-180+angleOffset)
      I=-180+angleOffset;
    if(speed<speedTarget/3*2){
      I=150*0.8+I*0.2;
    }
    //control=-speedErr*speedP+I;
    int fix=0;
    if(speedTarget-speed<600&&speed-lastSpeed>80){
      fix=(speed-lastSpeed-80)*0.01;
      if(fix>50)
        fix=50;
    }
    control=PControl+I+fix;
    if(control-oldControl>170){
      control=oldControl+170;
    }
    else if(control-oldControl<-110){
      control=oldControl-110;
    }
    if(control>230+angleOffset)
      control=230+angleOffset;
    else if(control<-340+angleOffset)
      control=-340+angleOffset;
    //uartAddWare(I,7);
    /*if(I>600)
      I=600;
    else if(I<-600)
      I=-600;
    control=-speedErr*speedP+I;
    if(control>600)
      control=600;
    else if(control<-600)
      control=-600;*/
    lastSpeed=speed;
  }
  //control=control*0.8+oldControl*0.2;
  
  uartAddWare(speedTarget,4);
  uartAddWare(speed,5);
  //uartAddWare(control,7);
  
  int output=0;
  
  output=oldControl+(control-oldControl)*(interruptTime%20)/20;
  
  return output;
}

int speedLever=1;
static float speedTs[3]={2500.0,2700.0,2900.0};

int speedTarget(){
  static int slt=350;
  if(circuitSlowDownEnabled&&circuitSlowDownMode){
    slt--;
    if(slt<=0)
      circuitSlowDownMode=0;
    ring();
    if(speed>2500)
      return (2500+speed*3)/4;
    else{
      if(2500<speed+200)
        return 2500;
      return speed+(2500-speed-200)/3*2+200;
    }
  }
  else{
    slt=600;
  }
  float speedT=speedTs[speedLever-1];
  static int runCount=0;
  if(stopMode&&isRun){
    return 10000;
  }
  if(!isRun)
    return 0;
  if(speedT<speed+200)
    return speedT;
  return speed+(speedT-speed-200)/3*2+200;
}

