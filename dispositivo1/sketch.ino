#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>

// ===== CONFIG SENSORES =====
#define DHTPIN 13
#define MQ135PIN 34

DHTesp dht;

// ===== WIFI =====
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// ===== MQTT =====
const char* mqtt_server = "broker.hivemq.com";
const char* topic_clima = "mackiot/sensor/clima";
const char* topic_gas   = "mackiot/sensor/gas";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 40) {
    delay(500);
    Serial.print(".");
    tentativas++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado!");
  } else {
    Serial.println("\nFalha no WiFi!");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    if (client.connect("ESP32_Dispositivo1")) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  dht.setup(DHTPIN, DHTesp::DHT22);
  delay(2000);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  // ===== DHT22 =====
  delay(dht.getMinimumSamplingPeriod());
  TempAndHumidity data = dht.getTempAndHumidity();
  float temp = data.temperature;
  float umid = data.humidity;

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" Umid: ");
  Serial.println(umid);

  if (!isnan(temp) && !isnan(umid)) {
    StaticJsonDocument<64> docClima;
    docClima["temperatura"] = temp;
    docClima["umidade"] = umid;

    char bufClima[64];
    serializeJson(docClima, bufClima);
    client.publish(topic_clima, bufClima);

    Serial.print("Clima: ");
    Serial.println(bufClima);
  } else {
    Serial.println("DHT22 erro - leitura invalida!");
  }

  // ===== MQ-135 =====
 
  int gasRaw = analogRead(MQ135PIN);
  float ppm = map(gasRaw, 300, 1700, 100, 500);

  StaticJsonDocument<64> docGas;
  docGas["ppm"] = ppm;
  docGas["raw"] = gasRaw;

  char bufGas[64];
  serializeJson(docGas, bufGas);
  client.publish(topic_gas, bufGas);

  Serial.print("Gas: ");
  Serial.println(bufGas);

  delay(10000);
}
