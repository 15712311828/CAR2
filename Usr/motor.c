#include "motor.h"

//电机初始化
void motorInit(){
  ftm_pwm_init(FTM0, FTM_CH3,10*1000,200);
  ftm_pwm_init(FTM0, FTM_CH4,10*1000,200);
  ftm_pwm_init(FTM0, FTM_CH5,10*1000,200);
  ftm_pwm_init(FTM0, FTM_CH6,10*1000,200);
  ftm_pwm_init(FTM0, FTM_CH0,10*1000,200);
}

int speedDuty=0;
int dirDuty=0;
void changeSpeedDuty(int duty){
  speedDuty=duty;
  motorDuty(speedDuty-dirDuty,speedDuty+dirDuty);
}

void changeDirDuty(int duty){
  dirDuty=duty;
}
//电机改变占空比
void motorDuty(int duty1,int duty2){
  duty1=-duty1;
  duty2=-duty2;
  if(duty1>9300)
    duty1=9300;
  else if(duty1<-9300)
    duty1=-9300;
  if(duty2>9300)
    duty2=9300;
  else if(duty2<-9300)
    duty2=-9300;
  if(duty1>0){
    ftm_pwm_duty(FTM0,FTM_CH3,duty1+200); 
    ftm_pwm_duty(FTM0,FTM_CH0,duty1+200); 
    ftm_pwm_duty(FTM0,FTM_CH4,200); 
  }
  else{
    ftm_pwm_duty(FTM0,FTM_CH3,200); 
    ftm_pwm_duty(FTM0,FTM_CH0,200); 
    ftm_pwm_duty(FTM0,FTM_CH4,-duty1+200);
  }
  if(duty2>0){
    ftm_pwm_duty(FTM0,FTM_CH5,duty2+200); 
    ftm_pwm_duty(FTM0,FTM_CH6,200); 
  }
  else{
    ftm_pwm_duty(FTM0,FTM_CH5,200); 
    ftm_pwm_duty(FTM0,FTM_CH6,-duty2+200);
  }
}

void motorStart(){
}

void motorStop(){
}