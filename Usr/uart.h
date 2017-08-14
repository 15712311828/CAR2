#ifndef _UART_H_
#define _UART_H_

#include "include.h"

#define UARTINSTANCE UART4

void uartInit();

void uartAddWare(uint16_t value,int chl);

void uartSendWare();

void uartSendImage();


#endif //_UART_H_