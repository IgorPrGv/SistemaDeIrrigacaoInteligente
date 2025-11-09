#ifndef BROKER_H
#define BROKER_H

#include <WiFi.h>
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

extern WiFiClientSecure wifiTLS;
extern PubSubClient mqtt;

void connectMQTT();
void supabaseSetup(); 
void brokerLoop();    
void debug(String message);
void debug(String category, String message);

void sendSensorReadings();

#endif
