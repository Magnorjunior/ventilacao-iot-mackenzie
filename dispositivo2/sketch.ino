#include <WiFi.h>
#include <PubSubClient.h>

#define LED_R 18
#define LED_G 19
#define LED_B 21
#define BUZZER 14

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";
const char* topic_alerta = "mackiot/atuador/alerta";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  msg.trim();
  int nivel = msg.toInt();

  Serial.print("Nivel recebido: ");
  Serial.println(nivel);

  if (nivel == 0) {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
    digitalWrite(BUZZER, LOW);
    Serial.println("Status: ADEQUADO");
  } else if (nivel == 1) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, LOW);
    digitalWrite(BUZZER, LOW);
    Serial.println("Status: ATENÇÃO");
  } else if (nivel == 2) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, LOW);
    digitalWrite(BUZZER, HIGH);
    Serial.println("Status: CRÍTICO");
  }
}

void setup_wifi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando MQTT...");
    String clientId = "MacKIoT_D2_";
    clientId += WiFi.macAddress();
    clientId.replace(":", "");
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
      client.subscribe(topic_alerta);
      break;
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  digitalWrite(BUZZER, LOW);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.setKeepAlive(60);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  delay(10);
}
