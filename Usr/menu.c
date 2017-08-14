#include "menu.h"

#define menuItemRoot 0
#define nullItem 0xFF

uint8_t menuItemPos=0;
menuItem menuItems[100];

uint8_t showingItem;
int8_t chosen=0;

uint8_t startItem;
uint8_t info;
uint8_t voltage;
//�༭�˵�
void editMenuItem(){
  startItem=newMenuItem("singleCarStart",menuItemRoot,startRun);
  info=newMenuItem("info",menuItemRoot,NULL);
  voltage=newMenuItem("voltage:0.00",info,noClick);
}
//�˵���ʼ��
void menuInit(){
  LCD_Init();
  LCD_Fill(0x00);
  newMenuItem("",menuItemRoot,NULL);
  editMenuItem();
  showMenuItem(menuItemRoot);
  
  port_init(PTC2, ALT1 | IRQ_FALLING | PULLUP );
  port_init(PTC3, ALT1 | IRQ_FALLING | PULLUP );
  port_init(PTC4, ALT1 | IRQ_FALLING | PULLUP );
  port_init(PTC5, ALT1 | IRQ_FALLING | PULLUP );
  //set_vector_handler(PORTC_VECTORn ,PORTC_IRQHandler); 
  //enable_irq (PORTC_IRQn);
}
//����Ŀ
uint8_t newMenuItem(uint8_t str[],uint8_t father,itemCallback callback){
  menuItems[menuItemPos].firstChild=menuItems[menuItemPos].firstChild=nullItem;
  menuItems[menuItemPos].next=nullItem;
  menuItems[menuItemPos].father=father;
  menuItems[menuItemPos].childNum=0;
  if(menuItemPos!=menuItemRoot){
    if(menuItems[father].firstChild==nullItem){
      menuItems[father].firstChild=menuItems[father].lastChild=menuItemPos;
    }
    else{
      menuItems[menuItems[father].lastChild].next=menuItemPos;
      menuItems[father].lastChild=menuItemPos;
    }
    menuItems[father].childNum++;
  }
  menuItems[menuItemPos].callback=callback;
  strcpy(menuItems[menuItemPos].str+1,str);
  menuItems[menuItemPos].str[0]=' ';
  menuItemPos++;
  return menuItemPos-1;
}
//��ʾĳ��Ŀ�Ӳ˵�
void showMenuItem(uint8_t item){
  showingItem=item;
  LCD_Fill(0x00);
  int i=0;
  for(uint8_t it=menuItems[item].firstChild;it!=nullItem;it=menuItems[it].next){
    if(i==chosen){
      uint8_t str[17];
      strcpy(str,menuItems[it].str);
      str[0]='$';
      for(int j=1;j<15;j++){
        if(!str[j]){
          for(;j<15;j++)
            str[j]=' ';
          break;
        }
      }
      str[14]='$';
      LCD_P8x16Str(2,2*i,str);
    }
    else{
      LCD_P8x16Str(2,2*i,menuItems[it].str);
    }
    i+=1;
  }
}
//�����ƶ����
void up(){
  if(menuItems[showingItem].childNum==0){
    chosen=0;
    return;
  }
  chosen--;
  if(chosen<0)
    chosen=menuItems[showingItem].childNum-1;
  showMenuItem(showingItem);
}
//�����ƶ����
void down(){
  chosen++;
  if(chosen>=menuItems[showingItem].childNum)
    chosen=0;
  showMenuItem(showingItem);
}
//ȷ��
void comfirm(){
  uint8_t item=choosingItem();
  if(menuItems[item].callback){
    menuItems[item].callback();
    return;
  }
  if(menuItems[item].firstChild==nullItem)
    return;
  chosen=0;
  showMenuItem(item);
}
//�˳�
void cancel(){
  chosen=0;
  showMenuItem(menuItems[showingItem].father);
}
//ѡ�в˵�
uint8_t choosingItem(){
  if(menuItems[showingItem].childNum==0){
    return showingItem;
  }
  uint8_t item=menuItems[showingItem].firstChild;
  int temp=chosen;
  while(temp--){
    item=menuItems[item].next;
  }
}
//�����ж�
/*void PORTC_IRQHandler(){
  PORT_FUNC(C,2,up);
  PORT_FUNC(C,3,down);
  PORT_FUNC(C,4,comfirm);
  PORT_FUNC(C,5,cancel);
}*/

void noClick(){
  
}


  