#include "uart.h"

uint16_t ware[8];
//���ڳ�ʼ��
void uartInit(){
  uart_init(UARTINSTANCE,115200);
  for(int i=0;i<8;i++){
    ware[i]=0;
  }
}
//��Ӳ��εȴ�����
void uartAddWare(uint16_t value,int chl){
  ware[chl]=value;
}
//���Ͳ���
void uartSendWare(){
  vcan_sendware(ware, 16);
}
//����ͼ��
void uartSendImage(uint8 * image){
  vcan_sendimg(image, CAMERA_SIZE);
}