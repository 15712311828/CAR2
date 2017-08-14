#include"fuzzycontrol.h"
//模糊表
float control[11][8]={
  -5500,-5800,-5900,-6000,-6300,-6500,-7300,- 8000,
  -5400,-5400,-5500,-5600,-5800,-5800,-6300,- 6300,
  -4500,-4600,-4700,-4800,-4800,-4900,-5400,- 5800,
  -3700,-3700,-3800,-4000,-4200,-4300,-4100,- 4200,
  -1800,-1800,-1800,-1800,-1900,-1900,-2000,- 2000,
      0,    0,    0,    0,    0,    0,    0,     0,
   1800, 1800, 1800, 1800, 1900, 1900, 2000,  2000,
   3700, 3700, 3800, 4000, 4200, 4300, 4100,  4200,
   4500, 4600, 4700, 4800, 4800, 4900, 5400,  5800,
   5400, 5400, 5500, 5600, 5800, 5800, 6300,  6300,
   5500, 5800, 5900, 6000, 6300, 6500, 7300,  8000
};
//隶属度
float degree[11][8];
float errDegree[11];
float ecDegree[8];
//err分档
float errDistribution[11]={
  -32,-24,-18,-12,-6,0,6,12,18,24,32
};
//speed分档
float speedDistribution[8]={
  0,500,1000,1500,2000,2500,3000,3500
};

float fuzzyControlHander(float err,float ec,float speed){
  float controlD=230;
  static float lastErr=0;
  ec/=60;
  uartAddWare((int)(err*100),0);
  //uartAddWare((int)ec,1);
  if(err<errDistribution[0])
    err=errDistribution[0];
  else if(err>errDistribution[10])
    err=errDistribution[10];
  if(speed<speedDistribution[0])
    speed=speedDistribution[0];
  else if(speed>speedDistribution[7])
    speed=speedDistribution[7];
  //计算隶属度
  degreeHandler(err,speed);
  //返回控制量
  float temp=ec*controlD;
  float temp2=(err-lastErr)*30;
  /*if(temp>1500)
    temp=1500;
  else if(temp<-1500)
    temp=-1500;*/
  float control=controlCalculate()+temp+temp2;
  uartAddWare((int)control,2);
  uartAddWare((int)(control-temp),1);
  static int lastControl=0;
  if(control>lastControl+2000)
    control=lastControl+2000;
  if(control<lastControl-2000)
    control=lastControl-2000;
  control=control*0.8+lastControl*0.2;
  lastControl=control;
  lastErr=err;
  return control;
}
//隶属度计算
void degreeHandler(float err,float ec){
  for(int i=0;i<11;i++){
    errDegree[i]=errDegreeFunc(err,i);
  }
  for(int i=0;i<8;i++){
    ecDegree[i]=ecDegreeFunc(ec,i);
  }
  for(int i=0;i<11;i++){
    for(int j=0;j<8;j++){
      degree[i][j]=errDegree[i]*ecDegree[j];
    }
  }
}

//高斯型隶属度
float errDegreeFunc(float err,int index){
  return exp(-(err-errDistribution[index])*(err-errDistribution[index])/48);
}
//高斯型隶属度
float ecDegreeFunc(float ec,int index){
  return exp(-(ec-speedDistribution[index])*(ec-speedDistribution[index])/25000);//3
}
//计算控制量
float controlCalculate(){
  float ans=0;
  float degreeSum=0;
  for(int i=0;i<11;i++){
    for(int j=0;j<8;j++){
      ans+=(control[i][j])*degree[i][j];
      degreeSum+=degree[i][j];
    }
  }
  return ans/degreeSum;
}
