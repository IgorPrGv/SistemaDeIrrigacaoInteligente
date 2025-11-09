#ifndef ULTRASONICSENSOR_H
#define ULTRASSONICSENSOR_H

#include <Arduino.h>

// pinos e constantes
#define TRIG_PIN               32
#define ECHO_PIN               33
#define SOUND_SPEED_CM_PER_US  0.0343f
#define MAX_DISTANCE_CM        200
#define TANK_AREA_CM2          314.16f 

extern float distance;
extern float volume;
extern float waterPercent;

void setupUltrassonicSensor();

void triggerSensor();

float readDistanceCm();

float convertToVolumeL(float distance);

void ultrassonicSensor();

#endif 
