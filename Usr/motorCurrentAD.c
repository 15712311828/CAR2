#include "motorCurrentAD.h"

int current1=0,current2=0;

int currentOffset[2]={
  928,908,
};

void motorCurrentADInit(){
  adc_init(ADC1_SE4a);
  adc_init(ADC1_SE5a);
}

int motorCurrentMeasure(){
  int current;
  int temp1=adc_once(ADC1_SE4a, ADC_12bit);
  int temp2=adc_once(ADC1_SE5a, ADC_12bit);
  current=temp1+temp2-currentOffset[0]-currentOffset[1]; 
  return current;
}
