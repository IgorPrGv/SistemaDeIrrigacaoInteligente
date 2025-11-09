#ifndef DEEPSLEEPMANAGER_H
#define DEEPSLEEPMANAGER_H

#include <Arduino.h>
#include "driver/rtc_io.h"

#define uS_TO_MIN_FACTOR 60000000ULL  // μs para minutos
#define uS_TO_SEC_FACTOR 1000000ULL   // μs para segundos

#define DEEP_SLEEP_TIME_MIN 10       // Tempo entre ciclos
#define AWAKE_CYCLE_TIME_SEC 210      // Tempo aberto
#define SENSOR_READ_INTERVAL_SEC 35   // Espaço entre leituras

// Variáveis RTC
extern RTC_DATA_ATTR int bootCount;
extern RTC_DATA_ATTR time_t lastWakeTime;
extern RTC_DATA_ATTR bool pumpActivatedLastCycle;
extern RTC_DATA_ATTR int readingsDuringAwakeCycle;
extern RTC_DATA_ATTR unsigned long lastSensorReadTime;
extern RTC_DATA_ATTR unsigned long awakeCycleStartTime;
extern unsigned long lastMQTTMaintenance;

void print_wakeup_reason();
void prepareForDeepSleep();
void enterDeepSleep();
bool shouldGoBackToSleep();

#endif