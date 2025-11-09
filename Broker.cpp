#include "Broker.h"
#include "config.h"
#include "SoilMoistureSensor.h"
#include "UltrassonicSensor.h"
#include "BatterySensor.h"
#include "WaterPump.h"

// ===== Certificado do emqx Cloud  =====
// static const char EMQX_CA[] PROGMEM = R"EOF(
// -----BEGIN CERTIFICATE-----
// MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
// TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
// cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
// WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
// ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
// MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
// h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
// 0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
// A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
// T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
// B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
// B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
// KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
// OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
// jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
// qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
// rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
// HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
// hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
// ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
// 3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
// NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
// ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
// TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
// jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
// oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
// 4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
// mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
// emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
// -----END CERTIFICATE-----
// )EOF";

// Clientes
WiFiClientSecure wifiTLS;
PubSubClient mqtt(wifiTLS);

static void connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  debug("MQTT", "Conectando Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    debug("MQTT", ".");
  }
  debug("MQTT", "Wi-Fi conectado");
}

void connectMQTT() {
  if (mqtt.connected()){ mqtt.loop(); return;}
  wifiTLS.setInsecure();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setBufferSize(1024);
  debug("MQTT", "Conectando MQTT");
  while (!mqtt.connected()) {
    if (mqtt.connect("esp32Client", MQTT_USER, MQTT_PASS)) {
      debug("MQTT", "MQTT conectado");
    } else {
      String erro = "Falha MQTT rc= " + String(mqtt.state());
      debug("MQTT", erro);
      delay(2000);
    }
  }
}

void supabaseSetup() {
  connectWiFi();
  connectMQTT();
  debug("SISTEMA", "Conexões MQTT e Wi-Fi estabelecidas");
}

void debug(String message) {
  Serial.println(message);  
  if (mqtt.connected()) {
    mqtt.publish("esp32/IC-Nutec-IrrigationSystemDebug", message.c_str());
  }
}

void debug(String category, String message) {
  String formattedMessage = "[" + category + "] " + message;
  Serial.println(formattedMessage);
  if (mqtt.connected()) {
    mqtt.publish("esp32/IC-Nutec-IrrigationSystemDebug", formattedMessage.c_str());
  }
}

void sendSensorReadings() {
  connectMQTT();
  if (!mqtt.connected()) { 
    debug("MQTT", "Sem conexão (aguarda próximo ciclo)"); 
    return; 
  }

  StaticJsonDocument<512> doc;
  doc["soil_capacitance"] = moisture;
  doc["moisture_percent"] = moisturePercent;
  doc["moisture_degree"]  = moistureDegree;
  doc["distance_cm"]      = distance;
  doc["volume_L"]         = volume;
  doc["reservoir_percent"]= waterPercent;
  doc["battery_percent"]  = soc;
  doc["battery_voltage"]  = vbat;
  doc["pump_flag"]        = pump_flag;
  doc["pump_reason"]        = pump_reason;

  char buf[256];
  size_t n = serializeJson(doc, buf);

  if (mqtt.publish(MQTT_TOPIC, buf, n)) {
    debug("MQTT", "Dados dos sensores publicados");
  } else {
    debug("MQTT", "Falha ao publicar dados dos sensores");
  }
  mqtt.loop();
}

void brokerLoop() {
  if (WiFi.status() != WL_CONNECTED) connectWiFi();
  if (!mqtt.connected()) connectMQTT();
  mqtt.loop();
}
