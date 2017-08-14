#include "encoder.h"
//编码器初始化
void encoderInit(){
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}
//采集
int encoderMeasure(){
  int speed1 = ftm_quad_get(FTM1);
  ftm_quad_clean(FTM1);
  int speed2 = ftm_quad_get(FTM2);
  ftm_quad_clean(FTM2);
  
  speed2=-speed2;
  
  //if(speed2<0&&speed1>0)
  //  speed1=-speed1;
 
  //uartAddWare(speed1,6);
  //uartAddWare(speed2,7);
  
  return speed1+speed2;
}