#include "angleControl.h"

int angleM;

int angleControl(int angle){
  int static lastAngle=0;
  int static lastOffset=0;
  int static lastAngleM=0;
  float PControl[9]={-1800,-1200,-800,-400,0,400,800,1200,1800};//{-4000,-2600,-1300,-670,0,570,1300,2600,4000};
  float offset[9]={-500,-260,-130,-60,0,60,130,260,500};
  float fixControl[9]={400,300,200,100,0,-100,-200,-300,-400};
  float fixOffset[9]={-250,-150,-50,50,150,250,350,450,550};
  float angleD=0;//0.4;//0.01;//105;//0.82
  
  angleM=angleCalculate();
  int angleOffset=angle-angleM;
  angleCheck(angleOffset);
  
  int temp=0;
  if(angleOffset<=offset[0])
    temp=PControl[0];
  else if(angleOffset>=offset[8])
    temp=PControl[8];
  else{
    for(int i=0;i<8;i++){
      if(angleOffset>=offset[i]&&angleOffset<=offset[i+1]){
        temp=PControl[i]+(PControl[i+1]-PControl[i])*(angleOffset-offset[i])/(offset[i+1]-offset[i]);
      }
    }
  }
  
  int fix=0;
  if(angleM<=fixOffset[0])
    fix=fixControl[0];
  else if(angleM>=fixOffset[8])
    fix=fixControl[8];
  else{
    for(int i=0;i<8;i++){
      if(angleM>=fixOffset[i]&&angleM<=fixOffset[i+1]){
        fix=fixControl[i]+(fixControl[i+1]-fixControl[i])*(angleM-fixOffset[i])/(fixOffset[i+1]-fixOffset[i]);
      }
    }
  }

  int dControl=(angleM-lastAngleM)*angleD;//(angleOffset-lastOffset)*angleD;
  if(dControl>1000)
    dControl=1000;
  else if(dControl<-1000)
    dControl=-1000;
  int control=temp+dControl+fix;
  
  lastAngle=angle;
  lastOffset=angleOffset;
  lastAngleM=angleM;
  
  if(control>1200)
    control=1200;
  else if(control<-1200)
    control=-1200;
  
  //uartAddWare(temp,0);
  //uartAddWare(fix,1);
  //uartAddWare(dControl,2);
  uartAddWare(angle,6);
  //return dControl;
  return control;
}

int angleTarget(){
  if(isRun)
    return -5;
  return 20;
}

void angleCheck(int angle){
  static int c=0;
  if(angle>550||angle<-550){
    c++;
    if(c>=30)
      exceptionStop();
  }
  else{
    c=0;
  }
}