#include "dirControl.h"

int dirControl(){
  //�ϴ�ƫ��
  //static float lastOffset=0;
  int dirDuty;
  if(isImageNew){
    //����
    midLineHandler();
    //���������
    if(runNormalCheck()==0){
      return 0;
    }
    //����ƫ��
    float offset=averageOffset();
//if(!isStart)
      //�ָ�ͼ����������
      recoverImg();
    //ģ�����������
    int gyroY=gyroYMeasure();
    dirDuty=fuzzyControlHander(offset,gyroY,speed);
    //lastOffset=offset;
    isImageNew=0;
  }
  return dirDuty;
}
