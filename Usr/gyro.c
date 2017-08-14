#include "gyro.h"


void gyroInit(){
  adc_init(ADC1_SE6a);
  adc_init(ADC1_SE7a);
  adc_init(ADC1_SE15);
}

int gyroXMeasure(){
  int temp=adc_once(ADC1_SE6a, ADC_12bit)-1300;
  return -temp;
}

int gyroYMeasure(){
  return adc_once(ADC1_SE15, ADC_12bit)-3005;
}

int accelerometerMeasure(){
  return adc_once(ADC1_SE7a, ADC_12bit)-2925;
}