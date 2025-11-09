#include "BatterySensor.h"
#include "Broker.h"  

float vbat = 0.0f;
float soc = 0.0f;
float vbat2 = 0.0f; 

static const float R1 = 10000.0f;  
static const float R2 = 10000.0f;  
static const float VOLTAGE_DIVIDER_FACTOR = (R1 + R2) / R2; 

static const float CALIBRATION_FACTOR = 1.000f;  

struct BatteryLUT { float voltage; int percent; };
static const BatteryLUT LUT[] = {
  {4.20f, 100}, {4.10f, 90}, {4.00f, 80}, {3.90f, 70},
  {3.80f, 60}, {3.70f, 50}, {3.60f, 40}, {3.50f, 30},
  {3.40f, 20}, {3.30f, 10}, {3.20f, 5}, {3.00f, 0}
};
static const int LUT_SIZE = sizeof(LUT) / sizeof(LUT[0]);

void setupADC() {
  analogSetPinAttenuation(BAT_ADC_PIN, ADC_6db);
  analogSetPinAttenuation(BAT2_ADC_PIN, ADC_6db);  
}
  
float readBatteryVoltage() {
  const int NUM_SAMPLES = 16;
  uint32_t sum_mV = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum_mV += analogReadMilliVolts(BAT_ADC_PIN);
    delay(2);  
  }

  float voltageADC = (sum_mV / (float)NUM_SAMPLES) / 1000.0f;  
  float voltageBattery = voltageADC * VOLTAGE_DIVIDER_FACTOR * CALIBRATION_FACTOR;
  if (voltageBattery < 0) return 0.0f;
  return voltageBattery;
}

// interpolação linear
float calculateBatteryPercent(float voltage) {
  if (voltage >= LUT[0].voltage) return 100.0f;
  if (voltage <= LUT[LUT_SIZE - 1].voltage) return 0.0f;

  for (int i = 0; i < LUT_SIZE - 1; i++) {
    if (voltage <= LUT[i].voltage && voltage >= LUT[i+1].voltage) {
      float ratio = (voltage - LUT[i+1].voltage) / (LUT[i].voltage - LUT[i+1].voltage);
      return LUT[i+1].percent + ratio * (LUT[i].percent - LUT[i+1].percent);
    }
  }
  return 0.0f;
}

void setupBattery() {
  setupADC();
  debug("BATERIA", "Leitor da bateria pronto");
}

void batterySensor(){
  vbat = readBatteryVoltage();
  soc = calculateBatteryPercent(vbat);
  vbat2 = readBatteryVoltage();

  String mensagem = "Voltage: " + String(vbat, 3) + 
                   " V | SoC: " + String(soc, 1) +
                   " | Bateria2: " + String(vbat2, 3) + " V";
  debug("BATERIA", mensagem);
}
