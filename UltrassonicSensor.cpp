#include "UltrassonicSensor.h"
#include "Broker.h"  

float distance = 0.0f;
float volume   = 0.0f;
float waterPercent = 0.0f;

void setupUltrassonicSensor() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void triggerSensor() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
}

float readDistanceCm() {
  triggerSensor();
  long duration = pulseIn(ECHO_PIN, HIGH, 30000); 
  if (duration == 0) {
    return -1.0f;
  }

  return (duration * SOUND_SPEED_CM_PER_US) / 2.0f;
}

float readDistanceAverageCm(int readings = 10) {
  float sum = 0.0f;
  int validReadings = 0;
  
  for (int i = 0; i < readings; i++) {
    float currentDistance = readDistanceCm();

    if (currentDistance <= 0.0f) {
      currentDistance = 0.0f;
    }
    
    debug("ULTRASSOM", "Leitura " + String(i+1) + ": " + String(currentDistance, 2) + " cm");

    if (currentDistance > 0.0f && currentDistance < 50.0f) {
      sum += currentDistance;
      validReadings++;
    }
    
    delay(500); 
  }
  return sum / validReadings;
}

float convertToVolumeL(float dist) {
  // volume em cm³
  float cm3 = dist * TANK_AREA_CM2;
  float vol = cm3 / 1000.0f;
  // 0 cm = tanque cheio (maxL), maxima distancia cm = vazio, limite necessario para irrigar 10cm, limite maximo 5cm
  const float maxL = 10.995f;
  float result = maxL - vol;
  if (result < 0.0f)   result = 0.0f;
  if (result > maxL) result = maxL;
  return result;
}

float getReservoirPercent() {
  const float maxL = 10.995f;   // mesma capacidade máxima usada em convertToVolumeL
  if (volume <= 0.0f) return 0.0f;
  float pct = (volume / maxL) * 100.0f;
  if (pct > 100.0f) pct = 100.0f;
  return pct;
}


void ultrassonicSensor() {
  
  distance = readDistanceAverageCm(10);
  if (distance < 0.0f) {
    debug("ULTRASSOM", "Sem eco detectado.");
    distance = 0.0f;
    volume   = 0.0f;
    waterPercent = 0.0f;
    return;
  }

  volume = convertToVolumeL(distance);
  waterPercent = getReservoirPercent();

  String mensagem = "Distância: " + String(distance, 2) + 
                   " cm | Volume estimado: " + String(volume, 3) + " L | Porcentagem de água "
                   + String(waterPercent, 3) + " %";
  debug("ULTRASSOM", mensagem);
}
