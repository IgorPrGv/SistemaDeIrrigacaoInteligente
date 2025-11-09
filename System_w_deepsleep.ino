#include <Arduino.h>
#include <Wire.h>

#include "Broker.h"
#include "SoilMoistureSensor.h"
#include "UltrassonicSensor.h"
#include "BatterySensor.h"
#include "WaterPump.h"
#include "DeepSleep.h"

extern RTC_DATA_ATTR unsigned long lastSensorReadTime = 0;
extern RTC_DATA_ATTR unsigned long lastPumpActivationTime = 0;

void setup() {
  // Configurar CPU em baixa frequência
  setCpuFrequencyMhz(80);
  
  Serial.begin(9600);
  delay(1000);

  // Gerenciar boot count e tempo
  bootCount++;
  if (awakeCycleStartTime == 0) {
    awakeCycleStartTime = millis();
    debug("SISTEMA", "Iniciando novo ciclo");
  }

  debug("SISTEMA", "Boot: " + String(bootCount) + 
                   " | Leitura: " + String(readingsDuringAwakeCycle));

  if (readingsDuringAwakeCycle == 0) {
    supabaseSetup();
    print_wakeup_reason();
    setupSoilSensor();
    setupUltrassonicSensor();
    setupBattery();
    setupPump();
  }

 if (shouldGoBackToSleep()) {
    debug("SISTEMA", "Ciclo completo. " + 
                     String(readingsDuringAwakeCycle) + " leituras realizadas");
    
    prepareForDeepSleep();
    readingsDuringAwakeCycle = 0;
    awakeCycleStartTime = 0;
    lastSensorReadTime = 0;
    lastPumpActivationTime = 0;
    enterDeepSleep();
    return; // Sair do setup se for "dormir"
  }

  checkPumpTimeout();

  unsigned long currentTime = millis();
  unsigned long timeSinceLastRead = currentTime - lastSensorReadTime;
  
  if (timeSinceLastRead >= (SENSOR_READ_INTERVAL_SEC * 1000) || 
      lastSensorReadTime == 0) {
    

    if (isPumpActive()) {
      debug("SISTEMA", "Bomba ativa - adiando leitura");
    } else {
      debug("SISTEMA", "--- Leitura #" + String(readingsDuringAwakeCycle + 1) + " ---");
      brokerLoop();
      delay(200);
      soilSensor();
      delay(100);              
      ultrassonicSensor();
      delay(100);
      batterySensor();
      delay(100);
      evaluatePump();
      delay(100);
      sendSensorReadings();

      lastSensorReadTime = currentTime;
      readingsDuringAwakeCycle++;
    }
  }
}

void loop() {
  checkPumpTimeout();
  delay(SENSOR_READ_INTERVAL_SEC * 1000); // Esperar 30s
  setup(); // próxima leitura
}