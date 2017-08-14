#include "dirControl.h"

int dirControl(){
  //上次偏差
  //static float lastOffset=0;
  int dirDuty;
  if(isImageNew){
    //中线
    midLineHandler();
    //出赛道检测
    if(runNormalCheck()==0){
      return 0;
    }
    //计算偏差
    float offset=averageOffset();
//if(!isStart)
      //恢复图像用来发送
      recoverImg();
    //模糊计算控制量
    int gyroY=gyroYMeasure();
    dirDuty=fuzzyControlHander(offset,gyroY,speed);
    //lastOffset=offset;
    isImageNew=0;
  }
  return dirDuty;
}
