#include "camera.h"

uint8 imgbuff[CAMERA_SIZE]; 
uint8 whitemask[CAMERA_SIZE*8]; 

//����ͷ��ʼ��
void cameraInit(){
  //��ʼ�����ж�
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
//�ɼ�
void cameraGetImage(){
  camera_get_img();
}
//����ͷ�ж�
void PORTA_IRQHandler()
{
    uint8  n = 0;
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;                                   //���жϱ�־λ

    n = 29;                                             //���ж�
    if(flag & (1 << n))                                 //PTA29�����ж�
    {
        camera_vsync();
    }
#if 0             //ӥ��ֱ��ȫ�ٲɼ�������Ҫ���ж�
    n = 28;
    if(flag & (1 << n))                                 //PTA28�����ж�
    {
        camera_href();
    }
#endif
}
//DMA�ж�
void DMA0_IRQHandler()
{
    camera_dma();
}