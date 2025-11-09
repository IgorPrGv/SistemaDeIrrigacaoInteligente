#include "SoilMoistureSensor.h"
#include "Broker.h"

int moisture = 0;
int moisturePercent = 0;
const char* moistureDegree = "N/A";

void setupSoilSensor() {
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  analogSetAttenuation(ADC_11db);  
  debug("SOLO_ANALOG", "Sensor de umidade do solo analógico inicializado");
}

void soilSensor() {
  
  const int numReadings = 10;
  long sum = 0;
  
  for (int i = 0; i < numReadings; i++) {
    sum += analogRead(SOIL_MOISTURE_PIN);
    delay(2000);
  }
  
  moisture = sum / numReadings;
  
  // valor maior = mais seco
  moisturePercent = map(moisture, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);
  
  if (moisturePercent < 20) {
    moistureDegree = "Muito Seco";
  } else if (moisturePercent < 40) {
    moistureDegree = "Seco";
  } else if (moisturePercent < 60) {
    moistureDegree = "Ideal";
  } else if (moisturePercent < 80) {
    moistureDegree = "Úmido";
  } else {
    moistureDegree = "Encharcado";
  }
  
  String msg = "Valor: " + String(moisture) + 
               " | " + String(moisturePercent) + "%" +
               " | " + String(moistureDegree);
  debug("SOLO_ANALOG", msg);
}