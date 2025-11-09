#ifndef SOILMOISTURESENSOR_H
#define SOILMOISTURESENSOR_H

#include <Arduino.h>

#define SOIL_MOISTURE_PIN 36

#define DRY_VALUE 4000    
#define WET_VALUE 1396    

extern int moisture;
extern int moisturePercent;
extern const char* moistureDegree;

void setupSoilSensor();
void soilSensor();

#endif