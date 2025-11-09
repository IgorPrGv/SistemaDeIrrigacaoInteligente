#ifndef WATERPUMP_H
#define WATERPUMP_H

#include <Arduino.h>
#include "Broker.h"
#include "UltrassonicSensor.h"
#include "SoilMoistureSensor.h"

#define RELAY_PIN       16
#define SOIL_TARGET_ON  40.0f   // ativa se umidade < 30%
#define BAT_MIN_V       3.50f   // não ativa se bateria < 3.50V
#define RES_MIN_PCT     30.0f   // não ativa se reservatório < 50%
#define RES_MAX_PCT     90.0f   // ativa se reservatório < 90%
#define PUMP_RUN_TIME   25000UL // 30 s

extern bool        pump_flag;    
extern const char* pump_reason;  

void setupPump();

void evaluatePump();

void checkPumpTimeout();

void startPump(const char* pump_reason);

bool isPumpActive();

#endif
