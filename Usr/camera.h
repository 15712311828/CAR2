#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "common.h"
#include "include.h"

extern uint8 imgbuff[CAMERA_SIZE];
extern uint8 whitemask[CAMERA_SIZE*8];

void cameraInit();

void cameraGetImage();

void PORTA_IRQHandler();

void DMA0_IRQHandler();


#endif  //_CAMERA_H_