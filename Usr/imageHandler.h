#ifndef _IMAGE_HANDER_H_
#define _IMAGE_HANDER_H_


#include "common.h"
#include "include.h"

#define white 0
#define black 1
#define W 80
#define H 60

extern int isImageNew;

extern int circuitExpected;
extern int circuitSlowDownMode;
extern int circuitSlowDownEnabled;

extern uint8_t picture[W*H];
extern uint8 recoverImage[CAMERA_SIZE];

void midLineHandler();
void findSideLine2();
void sideLineFix2();
void circuitOut2();
void circuitOut3();
void stopModeHandler();
void circuitSlowSown();
void findSideLine();
void sideLineAnalysis();
void sideLineFix();
void farMidFix();
void straightLineCheck();
void drawLine();
float averageOffset();
void midFix();
int runNormalCheck();
void copyImg();
void recoverImg();
void connect(int * line,int start,int end);
int getLeftFromMid(int line,int startPoint);
int getRightFromMid(int line,int startPoint);
int getLeftFromLeft(int line,int startPoint);
int getRightFromRight(int line,int startPoint);
int * analiseLine(int line,int start,int end);
int reliabilityCal(int line);


#endif  //_IMAGE_HANDER_H_