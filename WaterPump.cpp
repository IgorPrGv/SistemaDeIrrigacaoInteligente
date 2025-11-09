#include "WaterPump.h"

// Estado do ciclo
bool        pump_flag   = false;
const char* pump_reason = nullptr;
unsigned long pumpStartTime = 0;

static inline void pumpOn()  { digitalWrite(RELAY_PIN, HIGH); }
static inline void pumpOff() { digitalWrite(RELAY_PIN, LOW);  }

void setupPump() {
  pinMode(RELAY_PIN, OUTPUT);
  pumpOff();
}

void startPump(const char* reason) {
  debug("BOMBA", "Tentativa de acionamento: " + String(reason));
  
  if (waterPercent < RES_MIN_PCT || waterPercent == 100.0f || distance == 0.0f) {
    debug("BOMBA", "BLOQUEADA - Reservatório vazio: " + String(waterPercent) + "% < " + String(RES_MIN_PCT) + "%");
    pump_reason = "EMPTY_TANK";
    pump_flag = false;
    return;
  }

  pump_flag   = true;
  pump_reason = reason;
  pumpStartTime = millis(); 

  pumpOn();  
  debug("BOMBA", "Ligada ");
}

void checkPumpTimeout() {
  if (pump_flag && (millis() - pumpStartTime >= PUMP_RUN_TIME)) {
    pumpOff();
    pump_flag = false;
    debug("BOMBA", "Desligada após segundos");
    pump_reason = nullptr;
  }
}

void evaluatePump() {

  checkPumpTimeout();

  if (pump_flag) {
    return;
  }

  if (waterPercent > RES_MAX_PCT && waterPercent <= 95.0){
    startPump("MAX_LEVEL");
    return;
  }

  if (moisturePercent < SOIL_TARGET_ON) {
    startPump("LOW_SOIL");
    return;
  }
}

// Função para verificar se a bomba está ativa
bool isPumpActive() {
  return pump_flag;
}

