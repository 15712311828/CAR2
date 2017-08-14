#include "uart.h"

uint16_t ware[8];
//串口初始化
void uartInit(){
  uart_init(UARTINSTANCE,115200);
  for(int i=0;i<8;i++){
    ware[i]=0;
  }
}
//添加波形等待发送
void uartAddWare(uint16_t value,int chl){
  ware[chl]=value;
}
//发送波形
void uartSendWare(){
  vcan_sendware(ware, 16);
}
//发送图像
void uartSendImage(uint8 * image){
  vcan_sendimg(image, CAMERA_SIZE);
}