#include "key.h"

PTXn_e KEY[6] = {PTD7,PTC2,PTC3,PTC5,PTNULL,PTNULL};
KeyCallback keyHandler[6]={0};
//°´¼ü³õÊ¼»¯
void keyInit(){
  for(int i=0;i<6;i++){
    if(KEY[i]!=PTNULL){
      port_init(KEY[i], ALT1 | IRQ_FALLING | PULLUP );
    }
  }
  set_vector_handler(PORTD_VECTORn ,PORTD_IRQHandler);
  enable_irq (PORTD_IRQn);  
  set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler);
  enable_irq (PORTC_IRQn); 
}

void keyCallbackInstall(KeyInstance key,KeyCallback keyCallback){
  keyHandler[key]=keyCallback;
}

void PORTD_IRQHandler(){
  if(keyHandler[0]){
    PORT_FUNC(D,7,keyHandler[0]);
  }
}
void PORTC_IRQHandler(){
  if(keyHandler[1]){
    PORT_FUNC(C,2,keyHandler[1]);
  }
  if(keyHandler[2]){
    PORT_FUNC(C,3,keyHandler[2]);
  }
  if(keyHandler[3]){
    PORT_FUNC(C,5,keyHandler[3]);
  }
}
