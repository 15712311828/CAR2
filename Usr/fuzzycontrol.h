#ifndef _FUZZYCONTROL_H_
#define _FUZZYCONTROL_H_


#include "common.h"
#include "include.h"
/*
float fuzzyPIDHander(float err,float ec);
*/

float fuzzyControlHander(float err,float ec,float speed);
void degreeHandler(float err,float ec);
float errDegreeFunc(float err,int index);
float ecDegreeFunc(float ec,int index);
float controlCalculate();



#endif  //_FUZZYCONTROL_H_