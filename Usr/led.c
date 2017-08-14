#include"led.h"

PTXn_e LED[6] = {PTB20,PTB21,PTB22,PTB23,PTNULL,PTNULL};
//�Ƴ�ʼ��
void ledInit(){
  for(int i=0;i<6;i++){
    if(LED[i]!=PTNULL){
      gpio_init(LED[i],GPO,1);
    }
  }
}
//����
void ledOn(LedInstance ledInstance){
  gpio_set(LED[ledInstance],0);
}
//�ر�
void ledOff(LedInstance ledInstance){
  gpio_set(LED[ledInstance],1);
}
//��ת
void ledTurn(LedInstance ledInstance){
  gpio_turn(LED[ledInstance]);
}