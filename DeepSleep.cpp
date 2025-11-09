#include "DeepSleep.h"
#include "Broker.h"
#include "WaterPump.h"

// Variáveis RTC
RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR time_t lastWakeTime = 0;
RTC_DATA_ATTR bool pumpActivatedLastCycle = false;
RTC_DATA_ATTR int readingsDuringAwakeCycle = 0;
RTC_DATA_ATTR unsigned long awakeCycleStartTime = 0;

void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
  
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_TIMER: 
      debug("DEEPSLEEP", "Acordou por timer (15 min)");
      break;
    default: 
      debug("DEEPSLEEP", "Acordou por reinicialização");
      break;
  }
}

bool shouldGoBackToSleep() {
  unsigned long currentAwakeTime = millis() - awakeCycleStartTime;
  unsigned long awakeTimeSeconds = currentAwakeTime / 1000;
  
  debug("DEEPSLEEP", "Tempo acordado: " + String(awakeTimeSeconds) + "s/" + 
                    String(AWAKE_CYCLE_TIME_SEC) + "s");
  
  return (awakeTimeSeconds >= AWAKE_CYCLE_TIME_SEC);
}

void prepareForDeepSleep() {
  debug("DEEPSLEEP", "Preparando para deep sleep...");
  
  pumpActivatedLastCycle = pump_flag;
  digitalWrite(RELAY_PIN, LOW);
  
  // Isolando pins GPIO
  rtc_gpio_isolate(GPIO_NUM_16);
  rtc_gpio_isolate(GPIO_NUM_36);
  rtc_gpio_isolate(GPIO_NUM_34);
  
  // wake-up por timer
  esp_sleep_enable_timer_wakeup(DEEP_SLEEP_TIME_MIN * uS_TO_MIN_FACTOR);
}

void enterDeepSleep() {
  Serial.flush();
  delay(100);
  esp_deep_sleep_start();
}