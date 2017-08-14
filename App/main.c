

#include "common.h"
#include "include.h"


void interrupt(){ 
  static int leverTick=0;
  if(interruptTime%100==0){
    if(leverTick<speedLever){
      ledOn(led1);
      leverTick++;
    }
    else{
      leverTick=0;
    }
  }
  else if(interruptTime%100==50){
    ledOff(led1);
  }
  if(circuitExpected){
    ledOn(led3);
    ledOff(led2);
  }
  else{
    ledOn(led2);
    ledOff(led3);
  }
  static int circuitSlowDownUnenabledTick=200;
  if(circuitSlowDownUnenabledTick&&!circuitSlowDownEnabled){
    ledOn(led0);
    ledOn(led1);
    ledOn(led2);
    ledOn(led3);
    circuitSlowDownUnenabledTick--;
  }
  static int angleTarget=0;
  static int currentTarget=0;
  angleTarget=speedControl(speedTarget());
  //angleTarget=130;
  currentTarget=angleControl(angleTarget);//+speedControl(speedTarget());//+speedControl(speedTarget());
  //currentTarget=500;
  int duty=currentControl(currentTarget);
  //if(isStart)duty=2000;
  changeSpeedDuty(duty);
}

void keyStartCallback(){
  startRun();
}

void keyChangeSpeedCallback(){
  if(isStart)
    return;
  speedLever++;
  if(speedLever>3)
    speedLever=1;
}

void keyChangeCircuitDirCallback(){
  if(isStart)
    return;
  circuitExpected=!circuitExpected;
}

void keycircuitSlowDownUneanabledCallback(){
  if(isStart)
    return;
  circuitSlowDownEnabled=0;
}

void main()
{
  motorInit();
  ledInit();
  uartInit();
  cameraInit();
  systemInit();
  encoderInit();
  keyInit();
  gyroInit();
  //menuInit();
  motorCurrentADInit();
  //���ȼ�����
  NVIC_SetPriorityGrouping(4);
  //���������ص�����
  keyCallbackInstall(key0,keyStartCallback);
  keyCallbackInstall(key1,keyChangeSpeedCallback);
  keyCallbackInstall(key2,keyChangeCircuitDirCallback);
  keyCallbackInstall(key3,keycircuitSlowDownUneanabledCallback);
  //�жϺ���
  systemInterruptCallbackInstall(interrupt);
  //���ж�
  systemInterruptOn();
  while(1)
  {
    //�ɼ�ͼ��
    cameraGetImage();
    //��ѹͼ��
    copyImg();
    //����
    int dirDuty=dirControl();
    //dirDuty*=0.5+speed/2000*1.1;
    //dirDuty=startTime>1000?-500:0;
    if(isStart){
      changeDirDuty(dirDuty);
    }
    if(!isRun){
      //����ͼ��
      uartSendImage(recoverImage);    
    }
    //���Ͳ���
    uartSendWare(); 
  }
}

