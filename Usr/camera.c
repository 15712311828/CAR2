#include "camera.h"

uint8 imgbuff[CAMERA_SIZE]; 
uint8 whitemask[CAMERA_SIZE*8]; 

//摄像头初始化
void cameraInit(){
  //初始化及中断
  camera_init(imgbuff);
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);
  NVIC_SetPriority(PORTA_IRQn, 0x0);
  NVIC_SetPriority(DMA0_IRQn, 0x1);
  
  memset(whitemask,0,sizeof(whitemask));
  for(int i=59;i>=53;i--){
    for(int j=25;j<=55;j++){
      whitemask[i*80+j]=1;
    }
  }
}
//采集
void cameraGetImage(){
  camera_get_img();
}
//摄像头中断
void PORTA_IRQHandler()
{
    uint8  n = 0;
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //清中断标志位

    n = 29;                                             //场中断
    if(flag & (1 << n))                                 //PTA29触发中断
    {
        camera_vsync();
    }
#if 0             //鹰眼直接全速采集，不需要行中断
    n = 28;
    if(flag & (1 << n))                                 //PTA28触发中断
    {
        camera_href();
    }
#endif
}
//DMA中断
void DMA0_IRQHandler()
{
    camera_dma();
}