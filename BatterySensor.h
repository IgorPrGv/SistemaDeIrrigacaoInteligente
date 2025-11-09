#ifndef BATTERYSENSOR_H
#define BATTERYSENSOR_H

#include <Arduino.h>

#define BAT_ADC_PIN 34
#define BAT2_ADC_PIN 4
  

extern float vbat, soc;
extern float vbat2;

void setupADC();

float readBatteryVoltage();

float calculateBatteryPercent(float voltage);

void setupBattery();

void batterySensor();

#endif 