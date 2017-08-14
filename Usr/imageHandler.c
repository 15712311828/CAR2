#include "imageHandler.h"


int isImageNew;
//行信息
int right[H];
int left[H];
int inRight[H];
int inLeft[H];
int mid[H];
int length[H];
int startFind[H]={40};
int reliability[H];
//环路方向
int circuitExpected=1;
int circuitSlowDownMode=0;
int circuitSlowDownEnabled=1;
//赛道类型
//0 普通  1 十字 2 入环 3 出环
int trackType=0;
int flag=0;
//计算中线
void midLineHandler(){
  //findSideLine();
  //sideLineAnalysis();
  //sideLineFix();
  //farMidFix();
  flag=1;
  findSideLine2();
  if(flag){
    circuitOut3();
    sideLineFix2();
  }
  if(circuitSlowDownMode){
    for(int i=H-2;i>=10;i--){
      if(i==10)
        circuitSlowDownMode=0;
      if(right[i]-right[i+1]<=0&&left[i]-left[i+1]>=0){
        continue;
      }
      else
        break;
    }
  }
  drawLine();
}

void findSideLine2(){
  if(stopMode){
    stopModeHandler();
    return;
  }
  for(int i=H-1;i>=H-6;i--){
    right[i]=getRightFromMid(i,W/2);
    left[i]=getLeftFromMid(i,W/2);
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
    reliability[i]=reliabilityCal(i);
  }
  for(int i=H-7;i>=0;i--){
    int * sides=analiseLine(i,left[i+1],right[i+1]);
    if(sides[0]==0){
      int temp=mid[i+1]>W/2?W-1:0;
      for(int j=i;j>=0;j--){
        right[j]=left[j]=mid[j]=temp;
        length[j]=right[j]-left[j];
        reliability[j]=0;
      }
      break;
    }
    else if(sides[0]==2){
      left[i]=sides[1];
      right[i]=sides[2];
      mid[i]=(right[i]+left[i])>>1;
      length[i]=right[i]-left[i];
      reliability[i]=reliabilityCal(i);
    }
    else{
      if(sides[0]>8){
        stopModeHandler();
        if(startTime>3000&&i>35)
          stopMode=1;
        return;
      }
      float reliableMid[6];
      float reliableLine[6];
      if(i>=20&&sides[1]!=0&&sides[sides[0]]!=W-1){
        if(sides[0]==6){
          if(sides[4]-sides[3]<12){
            sides[4]=sides[6];
            sides[3]=sides[5];
            sides[0]=4;
          }
        }
        if((sides[2]-sides[1]<17||sides[4]-sides[3]<17)&&abs(sides[1]-left[i+1])<=2&&abs(sides[4]-right[i+1])<=2&&length[i+1]<55){
          //if(sides[2]-sides[1]<17&&sides[4]-sides[3]>=17){
          if((float)(sides[2]-sides[1])/(sides[4]-sides[3])<0.5||(sides[2]-sides[1]<17&&sides[4]-sides[3]>=17)){
            while(picture[(i-1)*W+sides[3]]==black)
              sides[3]++;
            left[i]=sides[3]+3;
            right[i]=sides[4];
            mid[i]=(right[i]+left[i])>>1;
            length[i]=right[i]-left[i];
            reliability[i]=1;
            for(int j=i+1;j<=H-1;j++){
              if(left[j-1]-1<left[j])
                break;
              left[j]=left[j-1];
              if(j%2)
                left[j]--;
              mid[j]=(right[j]+left[j])>>1;
              length[j]=right[j]-left[j];
            }
            continue;
          }
          //else if(sides[2]-sides[1]>=17&&sides[4]-sides[3]<17){
          else if((float)(sides[2]-sides[1])/(sides[4]-sides[3])>2||(sides[2]-sides[1]>=17&&sides[4]-sides[3]<17)){
            while(picture[(i-1)*W+sides[2]]==black)
              sides[2]--;
            left[i]=sides[1];
            right[i]=sides[2]-3;
            mid[i]=(right[i]+left[i])>>1;
            length[i]=right[i]-left[i];
            reliability[i]=1;
            for(int j=i+1;j<=H-1;j++){
              if(right[j-1]+1>right[j])
                break;
              right[j]=right[j-1];
              if(j%2)
                right[j]++;
              mid[j]=(right[j]+left[j])>>1;
              length[j]=right[j]-left[j];
            }
            continue;
          }
        }
      }
      int pos=0;
      for(int j=i+1;j<=H-1;j++){
        if(reliability[j]){
          reliableMid[pos]=mid[j];
          reliableLine[pos]=j;
          pos++;
          if(pos>=6)
            break;
        }
      }
      int predicMid=0;
      if(pos<=4){
        predicMid=W/2;
      }
      else{
        predicMid=reliableMid[pos-1]+(reliableMid[1]-reliableMid[pos-1])*(i-reliableLine[1])/(reliableLine[1]-reliableLine[pos-1]);
      }
      if(abs(predicMid-40)<16&&predicMid!=40&&sides[2]-sides[1]>15&&sides[4]-sides[3]>15&&picture[W*i+predicMid]==black){
        circuitSlowDownMode=1;
        ring();
      }
      int off=1000;
      int circuitFix=circuitExpected==1?8:-8;
      int circuitFix2=circuitExpected==1?12:-12;
      for(int j=1;j<sides[0];j+=2){
        int l=sides[j];
        int r=sides[j+1];
        int len=r-l;
        if(r-l<5&&l!=0&&r!=W-1)
          continue;
        if((l==0||r==W-1)&&r-l<20){
          if(l==0){
            l=r-20;
          }
          else{
            r=l+20;
          }
        }
        int midT=(l+r)/2;
        if(abs(midT-predicMid)<off+circuitFix||(abs(midT-predicMid)<off+circuitFix2&&r-l>23)){
          //if(len>=20&&length[i]>=20&&!circuitExpected){
          //  continue;
          //}
          off=abs(midT-predicMid);
          left[i]=l>=0?l:0;
          right[i]=r<=W-1?r:W-1;
          mid[i]=(right[i]+left[i])/2;
          length[i]=right[i]-left[i];
          reliability[i]=1;
        }
      }
    }
  }
}

void sideLineFix2(){
  for(int i=H-1;i>=0;i--){
    int start,end;
    while(reliability[i]&&i>0)
      i--;
    if(i==0)
      break;
    start=i+1;
    if(i<=H-3&&reliability[i+2])
      start=i+2;
    if(start>=H)
      start=H-1;
    while(!reliability[i]&&i>0)
      i--;
    if(i==0)
      break;
    int loop=4;
    while(loop--){
      i--;
      if(i==0||!reliability[i-1])
        break;
    }
    end=i;
    connect(left,start,end);
    connect(right,start,end);
  }
  for(int i=H-1;i>=0;i--){
    mid[i]=(left[i]+right[i])/2;
  }
}

void circuitOut2(){
  int i=H-1;
  while(reliability[i]&&right[i]!=W-1&&left[i]!=0)
    i--;
  if(i<40)
    return;
  int llost=0;
  int rlost=0;
  for(int j=i;j>i-20;j--){
    if(right[j]==W-1)
      rlost=j;
    if(left[j]==0)
      llost=j;
  }
  if(llost&&rlost||llost==rlost)
    return;
  if(llost){
    
  }
  else{
    
  }
  picture[50*W+40]=!picture[50*W+40];
  picture[50*W+39]=!picture[50*W+39];
  picture[50*W+41]=!picture[50*W+41];
  if(llost){
    while(left[llost]==0)
      llost--;
    if(left[llost]<20||llost<0)
      return;
    left[llost]=0;
    right[llost]=getRightFromMid(llost,1);
    mid[llost]=(right[llost]+left[llost])>>1;
    length[llost]=right[llost]-left[llost];
    reliability[llost]=1;
    for(int k=llost;k>=0;k--){
      left[k]=0;
      right[k]=right[k+1]==0?0:getRightFromMid(k,0);
      mid[k]=(right[k]+left[k])>>1;
    length[k]=right[k]-left[k];
    reliability[k]=1;
    }
  }
  if(rlost){
    while(right[rlost]==W-1)
      rlost--;
    if(right[rlost]>W-20||rlost<0)
      return;
    left[rlost]=getLeftFromMid(rlost,W-2);
    right[rlost]=W-1;
    mid[rlost]=(right[rlost]+left[rlost])>>1;
    length[rlost]=right[rlost]-left[rlost];
    reliability[rlost]=1;
    for(int k=rlost;k>=0;k--){
      right[k]=W-1;
      left[k]=right[k+1]==W-1?W-1:getLeftFromMid(k,W-1);
      mid[k]=(right[k]+left[k])>>1;
    length[k]=right[k]-left[k];
    reliability[k]=1;
    }
  }
}

void circuitOut3(){
  static int dir=0;
  if(dir>0)
    dir--;
  if(dir<0)
    dir++;
  int i=H-1;
  while(reliability[i]&&right[i]!=W-1&&left[i]!=0)
    i--;
  if(i<40)
    return;
  i-=2;
  int llost=0;
  int rlost=0;
  int f=0;
  for(int j=0;i>0&&j<7;i--,j++){
    if(llost&&rlost)
      break;
    if(left[i]==0&&!llost)
      llost=i;
    if(right[i]==W-1&&!rlost)
      rlost=i;
    if((left[i-1]>left[i]+1&&!llost)||(right[i-1]<right[i]-1&&!rlost)){
      f++;
      if(f>=2)
        return;
    }
  }
  for(;i>=0;i--){
    if((llost&&rlost)||(llost&&abs(right[i]-right[i+1]>20)&&right[i]!=left[i])||(rlost&&abs(left[i]-left[i+1]>20)&&right[i]!=left[i]))
      break;
    if(left[i]==0&&!llost)
      llost=i;
    if(right[i]==W-1&&!rlost)
      rlost=i;
    if(right[i]==left[i])
      return;
  }
  if(i<=20)
    return;
  int c=0;
  for(;i>0;i--){
    if(left[i]!=0&&right[i]!=W-1){
      c++;
      if(c==3)
        return;
    }
    if((left[i+1]!=0&&left[i]-left[i+1]>25)||(right[i+1]!=W-1&&right[i]-right[i+1]<-25))
       return;
    if(left[i]==right[i])
      break;
  }
  if(dir==0&&length[H-1]>60)
    return;
  //picture[50*W+40]=!picture[50*W+40];
  //picture[50*W+39]=!picture[50*W+39];
  //picture[50*W+41]=!picture[50*W+41];
  ring();
  if(!dir){
    if(mid[0]==0)
      dir=2;
    else
      dir=-2;
  }
  else{
    if(dir>0)
      dir=4;
    if(dir<0)
      dir=-4;
  }
  i=H-2;
  while(reliability[i]&&right[i]!=W-1&&left[i]!=0)
    i--;
  if(dir>0){
    int rf=0;
    for(;i>0;i--){
      right[i]=right[i+1]==W-1?W-1:getRightFromRight(i,W-1);
      left[i]=getLeftFromMid(i,right[i]-2);
      if(left[i]<left[i+1])
        left[i]=left[i+1];
      mid[i]=(right[i]+left[i])>>1;
      length[i]=right[i]-left[i];
      if(!rf&&(left[i]-left[i+1]>20||length[i]<20||(right[i]==W-1&&left[i]>20)))
        rf=1;
      reliability[i]=rf;
    }
  }
  if(dir<0){
    int rf=0;
    for(;i>0;i--){
      left[i]=left[i+1]==0?0:getLeftFromLeft(i,0);
      right[i]=getRightFromMid(i,left[i]+2);
      if(right[i]>right[i+1])
        right[i]=right[i+1];
      mid[i]=(right[i]+left[i])>>1;
      length[i]=right[i]-left[i];
      if(!rf&&(right[i]-right[i+1]<-20||length[i]<20||(left[i]==0&&right[i]<W-21)))
        rf=1;
      reliability[i]=rf;
    }
  }
}

void stopModeHandler(){
  flag=0;
  //ring();
  for(int i=H-1;i>=H-6;i--){
    int r=W-1;
    int l=0;
    while(picture[i*W+r]==white&&r>=0){
      r--;
    }
    while(picture[i*W+l]==white&&l<=H-1){
      l++;
    }
    if(r<W/2)
      r=W-1;
    if(l>W/2)
      l=0;
    right[i]=getRightFromRight(i,r);
    left[i]=getLeftFromLeft(i,l);
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
    reliability[i]=1;
  }
  for(int i=H-7;i>=0;i--){
    right[i]=getRightFromRight(i,right[i+1]);
    left[i]=getLeftFromLeft(i,left[i+1]);
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
    reliability[i]=1;
  }
  static int tick=28;
  if(stopMode)
    tick--;
  if(!tick)
    exceptionStop();
}

void circuitSlowSown(){
  
}

//找边线
void findSideLine(){
  static int lastStartFind=W/2;
  for(int i=H-1;i>=H-6;i--){
    right[i]=getRightFromMid(i,W/2);
    left[i]=getLeftFromMid(i,W/2);
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
    inLeft[i]=left[i];
    inRight[i]=right[i];
    lastStartFind=mid[i];
  }
  int flag=1;
  int dir=2;
  for(int j=H-1;j>=H-6;j--){
    if((right[j]==W-1&&left[j]==0)||length[j]>60){
      flag=0;
      break;
    }
  }
  if(flag&&abs(mid[H-6]-mid[H-1])>5){
    if(mid[H-1]>=mid[H-2]&&mid[H-4]>=mid[H-5]&&mid[H-1]>mid[H-5]){
      dir=0;
    }
    if(mid[H-1]<=mid[H-2]&&mid[H-4]<=mid[H-5]&&mid[H-1]<mid[H-5]){
      dir=1;
    }
  }
  for(int i=H-7;i>=0;i--){
    int r1=getRightFromMid(i,lastStartFind);
    int l1=getLeftFromMid(i,lastStartFind);
    int r2=getRightFromRight(i,right[i+1]);
    int l2=getLeftFromLeft(i,left[i+1]);
    if(r1==r2&&l1==l2){
      right[i]=r1;
      left[i]=l1;
      inRight[i]=r1;
      inLeft[i]=l1;
    }
    else{
      if(r2<=l2){
        int temp;
        if(mid[i+1]>W/2){
          temp=W-1;
        }
        else{
          temp=0;
        }
        for(int j=i;j>=0;j--){
          left[j]=right[j]=mid[j]=inRight[j]=inLeft[j]=temp;
          length[j]=0;
        }
        break;
      }
      else if(l1<=l2||r1>=r2){
        right[i]=r2;
        left[i]=l2;
        inRight[i]=r2;
        inLeft[i]=l2;
      }
      else{
        r1=getRightFromRight(i,l2+1);
        l1=getLeftFromLeft(i,r2-1);
        right[i]=r2;
        left[i]=l2;
        inRight[i]=r1;
        inLeft[i]=l1;
        if(dir!=2&&right[i]-inRight[i]>10&&inLeft[i]-left[i]>10){
          if(dir==1){
            left[i]=inRight[i];
            inRight[i]=right[i];
            inLeft[i]=left[i];
          }
          if(dir==0){
            right[i]=inLeft[i];
            inRight[i]=right[i];
            inLeft[i]=left[i];
          }
        }
      }
    }
    mid[i]=(right[i]+left[i])>>1;
    lastStartFind=mid[i];
    length[i]=right[i]-left[i];
  }
  lastStartFind=mid[H-1];
  
  for(int i=1;i<H;i++){
    if(right[i-1]==left[i-1])
      continue;
    int r=getRightFromRight(i,right[i-1]);
    int l=getLeftFromLeft(i,left[i-1]);
    if(right[i]<=r&&left[i]<l){
      left[i]=l;
      int r=getRightFromRight(i,left[i]+1);
      if(r==right[i]){
        inRight[i]=right[i];
        inLeft[i]=left[i];
      }
      else{
        inRight[i]=r;
        inLeft[i]=getLeftFromLeft(i,right[i]-1);
      }
      mid[i]=(right[i]+left[i])>>1;
      length[i]=right[i]-left[i];
    }
    else if(left[i]>=l&&right[i]>r){
      right[i]=r;
      int r=getRightFromRight(i,left[i]+1);
      if(r==right[i]){
        inRight[i]=right[i];
        inLeft[i]=left[i];
      }
      else{
        inRight[i]=r;
        inLeft[i]=getLeftFromLeft(i,right[i]-1);
      }
      mid[i]=(right[i]+left[i])>>1;
      length[i]=right[i]-left[i];
    }
  }
}

void sideLineAnalysis(){
  /*
  trackType=0;
  for(int i=H-2;i>=1;i--){
    if(length[i]>length[i+1]&&(length[i-1]>=length[i]||right[i-1]==W-1||left[i-1]==0)){
      int temp=length[i+2];
      int j=i-10;
      if(mid[j]==0||mid[j]==W-1)
        j+=4;
      if(mid[j]==0||mid[j]==W-1)
        j+=2;
      for(;j>=0;j--){
        if(length[j]<temp&&length[j+1]<temp){
          connectMid(i+3,j);
          break;
        }
      }
      break;
    }
  }*/
  for(int i=H-1;i>=0;i--){
    if(right[i]!=inRight[i]||left[i]!=inLeft[i]){
      if((float)(right[i]-inLeft[i])/(inRight[i]-left[i])>2){
        for(;i>=0;i--){
          if(right[i]==inRight[i]&&left[i]==inLeft[i])
            break;
          left[i]=inLeft[i];
        }
      }
      else if((float)(inRight[i]-left[i])/(right[i]-inLeft[i])>2){
        for(;i>=0;i--){
          if(right[i]==inRight[i]&&left[i]==inLeft[i])
            break;
          right[i]=inRight[i];
        }
      }
      else{
        int flag=1;
        for(int j=H-1;j>=H-6;j--){
          if((right[i]==W-1&&left[i]==0)||length[i]<length[i-1]){
            flag=0;
            break;
          }
        }
        if(flag){
          int dir=mid[H-5]-mid[H-1];
          if(dir>10){
            for(;i>=0;i--){
              if(right[i]==inRight[i]&&left[i]==inLeft[i])
                break;
              left[i]=inLeft[i];
            }
          }
          else if(dir<-10){
            for(;i>=0;i--){
              if(right[i]==inRight[i]&&left[i]==inLeft[i])
                break;
              right[i]=inRight[i];
            }
          }
          else{
            flag=0;
          }
        }
        if(!flag){
          if(circuitExpected){
            for(;i>=0;i--){
              if(right[i]==inRight[i]&&left[i]==inLeft[i])
                break;
              left[i]=inLeft[i];
            }
          }
          else{
            for(;i>=0;i--){
              if(right[i]==inRight[i]&&left[i]==inLeft[i])
                break;
              right[i]=inRight[i];
            }
          }
        }
      }
    }
  }
  
  for(int i=H-1;i>=0;i--){
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
  }
}

void sideLineFix(){
  for(int i=H-1;i>=2;i--){
    if(i==H-1){
      if(length[i]==W-1){
        int j,k;
        for(j=H-1;i>=0;j--){
          if(right[j]!=W-1){
            int end=j-5;
            if(end<0)
              end=0;
            connect(right,H-1,end);
            break;
          }
        }
        for(k=H-1;k>=0;k--){
          if(left[k]!=0){
            int end=k-5;
            if(end<0)
              end=0;
            connect(left,H-1,end);
            break;
          }
        }
        if(j<k)
          i=j-1;
        else
          i=k-1;
      }
    }
    else{
      if(length[i]>length[i+1]&&(length[i-1]>length[i]||right[i-1]==W-1||left[i-1]==0)){
        for(int j=i-1;j>=0;j--){
          if(length[j]<length[i+1]){
            int start=i+4;
            if(start>H-1)
              start=H-1;
            int end=j-3;
            while((mid[end]==0||mid[end]==W-1)&&end<start)
              end++;
            if(end<0)
              end=0;
            connect(left,start,end);
            connect(right,start,end);
            i=j-1;
            break;
          }
        }
      }
    }
  }
  for(int i=H-1;i>=0;i--){
    mid[i]=(right[i]+left[i])>>1;
    length[i]=right[i]-left[i];
  }
}

void farMidFix(){
  int i;
  for(i=0;i<H-1;i++){
    if(mid[i]==0||mid[i]==W-1)
      continue;
    else
      break;
  }
  if(i!=0){
    if(mid[0]==0){
      for(int j=i-1;j>=0;j--){
        mid[j]=mid[j+1]-7;
        if(mid[j]<0){
          mid[j]=0;
          break;
        }
      }
    }
    else{
      for(int j=i-1;j>=0;j--){
        mid[j]=mid[j+1]+7;
        if(mid[j]>W-1){
          mid[j]=W-1;
          break;
        }
      }
    }
  }
}

//直道检测  用于加速
void straightLineCheck(){
  
}

//描线
void drawLine(){
  for(int i=0;i<H;i++){
    picture[i*W+mid[i]]=!picture[i*W+mid[i]];
    picture[i*W+left[i]]=!picture[i*W+left[i]];
    picture[i*W+right[i]]=!picture[i*W+right[i]];
    //picture[i*W+startFind[i]]=!picture[i*W+startFind[i]];
  }
  /*
  if(trackType==2){
    picture[59*W+81]=black;
    picture[59*W+80]=black;
    picture[59*W+79]=black;
    picture[60*W+79]=black;
    picture[61*W+79]=black;
    picture[61*W+80]=black;
    picture[61*W+81]=black;
    picture[60*W+81]=black;
  }
  if(trackType==1){
    picture[59*W+81]=black;
    picture[59*W+79]=black;
    picture[59*W+80]=black;
    picture[58*W+80]=black;
    picture[60*W+80]=black;
  }
  if(trackType==3){
    picture[59*W+81]=black;
    picture[59*W+80]=black;
    picture[59*W+79]=black;
    picture[60*W+79]=black;
    picture[61*W+79]=black;
    picture[61*W+80]=black;
    picture[61*W+81]=black;
    picture[61*W+82]=black;
    picture[61*W+83]=black;
    picture[60*W+81]=black;
  }*/
}

int fixedMid[60];
//计算偏差
float averageOffset(){
  //midFix();
  int angle=angleM;
  static int line_s[6][5]={
    12,29,39,43,47,//11,28,38,42,46,
    9,25,35,41,45,//8,24,34,40,44,
    6,21,30,35,38,
    6,18,28,34,37,//6,18,28,34,37,
    5,17,26,30,33,//5,17,26,30,33
    5,16,24,27,29
  };
  static int angle_s[6]={
    -200,-100,0,100,200,300
  };
  int line_c[5];
  if(angle<angle_s[0]){
    angle=angle_s[0];
  }
  else if(angle>angle_s[5]){
    angle=angle_s[5];
  }
  float rate=0;
  int i;
  for(i=0;i<5;i++){
    if(angle>=angle_s[i]&&angle<=angle_s[i+1]){
      rate=(float)(angle-angle_s[i])/(angle_s[i+1]-angle_s[i]);
      break;
    }
  }
  for(int j=0;j<5;j++){
    line_c[j]=line_s[i][j]+(line_s[i+1][j]-line_s[i][j])*rate;
  }
  int line[15];
  for(int i=0;i<5;i++){
    line[3*i]=line_c[i]-1;
    line[3*i+1]=line_c[i];
    line[3*i+2]=line_c[i]+1;
  }
  static float weight[8][15]={
    {5,5,5,0,0,0,0,0,0,0,0,0,0,0,0},  //0
    {5,5,5,1,0,0,0,0,0,0,0,0,0,0,0},   //500
    {5,5,5,5,2,0,0,0,0,0,0,0,0,0,0},    //1000
    {5,5,5,5,5,4,0,0,0,0,0,0,0,0,0},    //1500
    {7,8,10,6,5,4,4,2,2,0,0,0,0,0,0},    //2000
    {12,12,10,10,15,15,15,4,2,1,0,0,0,0,0},   //2500
    {12,12,12,12,12,15,15,4,4,3,2,1,0,0,0},   //3000
    {12,21,12,12,12,15,15,4,4,4,4,2,1,0,0}     //3500
  };
  static int speed_s[8]={
    0,500,1000,1500,2000,2500,3000,3500
  };
  float weight_c[15];
  int s=speed;
  if(s<speed_s[0]){
    s=speed_s[0];
  }
  else if(s>angle_s[7]){
    s=angle_s[7];
  }
  rate=0;
  for(i=0;i<7;i++){
    if(s>=speed_s[i]&&s<=speed_s[i+1]){
      rate=(float)(s-speed_s[i])/(speed_s[i+1]-speed_s[i]);
      break;
    }
  }
  for(int j=0;j<15;j++){
    weight_c[j]=weight[i][j]+(weight[i+1][j]-weight[i][j])*rate;
  }
  float weightSum=0;
  float offsetSum=0;
  for(int i=0;i<15;i++){
    offsetSum+=(mid[H-line[i]]-W/2)*weight_c[i];
    weightSum+=weight_c[i];
  }
  float offset=-(float)offsetSum/weightSum;
  
  return offset;
}

void midFix(){
  for(int i=H-1;i>=0;i--){
    if(i>H-25)
      fixedMid[i]=mid[i];
    else{
      int fix=(H-1-i-25)*0.8;
      if(mid[i]>W/2+fix){
        fixedMid[i]=mid[i]-fix;
      }
      else if(mid[i]<W/2-fix){
        fixedMid[i]=mid[i]+fix;
      }
      else{
        fixedMid[i]=W/2;
      }
    }
  }
}

int runNormalCheck(){
  return 1;
}

uint8_t picture[W*H];
uint8 recoverImage[CAMERA_SIZE];
//解压
void copyImg(){
  for(int i=0;i<W*H/8;i++){
    uint8_t temp=imgbuff[i];
    for(int j=0;j<8;j++){
      picture[8*i+j]=((temp>>(7-j))&0x01)&&(!whitemask[8*i+j]);
    }
  }
  isImageNew=1;
}
//恢复
void recoverImg(){
  for(int i=0;i<W*H/8;i++){
    uint8_t temp=0;
    for(int j=0;j<8;j++){
      temp+=picture[8*i+j]<<(7-j);
    }
    recoverImage[i]=temp;
  }
}
//连接
void connect(int * line,int start,int end){
  if(start<=end)
    return;
  for(int i=start-1;i>end;i--){
    int temp=line[start]+(line[end]-line[start])*(i-start)/(end-start);
    if((line==right&&temp>right[i])||(line==left&&temp<left[i]))
      continue;
    line[i]=temp;
  }
}
//从中间找左边界
int getLeftFromMid(int line,int startPoint)
{
  if(startPoint==0)
    return 0;
  while(picture[line*W+startPoint]==white){
    startPoint--;
    if(startPoint==0)
      break;
  }
  return startPoint;
}
//从中间找右边界
int getRightFromMid(int line,int startPoint)
{
  if(startPoint==W-1)
    return W-1;
  while(picture[line*W+startPoint]==white){
    startPoint++;
    if(startPoint==W-1)
      break;
  }
  return startPoint;
}

//从某位置找边界
int getLeftFromLeft(int line,int startPoint){
  if(startPoint==W-1){
    return W-1;
  }
  if(picture[line*W+startPoint]==white)
    return getLeftFromMid(line,startPoint);
  else{
    while(picture[line*W+startPoint]==black){
      if(startPoint==W-1){
        return W-1;
      }
      startPoint++;
    }
    return startPoint-1;
  }
}
//从某位置找边界
int getRightFromRight(int line,int startPoint){
  if(picture[line*W+startPoint]==white)
    return getRightFromMid(line,startPoint);
  else{
    while(picture[line*W+startPoint]==black){
      if(startPoint==0){
        return 0;
      }
      startPoint--;
    }
    return startPoint+1;
  }
}

int * analiseLine(int line,int start,int end){
  static int sides[30];
  int pos=0;
  int i=start;
  if(picture[line*W+start]==white){
    sides[pos+1]=getLeftFromMid(line,start);
    pos++;
    sides[pos+1]=getRightFromMid(line,start);
    pos++;
    i=sides[pos]+1;
  }
  for(;i<=end;i++){
    sides[pos+1]=getLeftFromLeft(line,i);
    if(sides[pos+1]>=right[line+1]){
      break;
    }
    pos++;
    sides[pos+1]=getRightFromMid(line,sides[pos]+1);
    pos++;
    i=sides[pos]+1;
  }
  sides[0]=pos;
  return sides;
}

int reliabilityCal(int line){
  if(line>=H-6){
    if(line<H-1&&reliability[line+1]==0)
      return 0;
    if(left[line]==0&&right[line]==W-1)
      return 0;
    if((line<H-1&&length[line]-length[line+1]>=2)||
       (line<H-2&&length[line]-length[line+1]>=1&&length[line+1]-length[line+2]>=1))
      return 0;
    return 1;
  }
  else{
    if(reliability[line+1]&&(abs(right[line]-right[line+1])<5&&abs(left[line]-left[line+1])<5)&&(length[line]-length[line+1]<2)&&(length[line]-length[line+1]<4)&&length[line]<40)
      return 1;
    if(line<45&&length[line+3]>=length[line+2]&&length[line+2]>=length[line+1]&&length[line+1]>=length[line]&&length[line]<35)
      return 1;
    return 0;
  }
}

