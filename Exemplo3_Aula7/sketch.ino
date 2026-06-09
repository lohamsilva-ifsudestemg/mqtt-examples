#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient IFSDEMG_Client;
PubSubClient client(IFSDEMG_Client);

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientID = "IFSDEMG_Botao_" + String(random(0, 1000));
    if (client.connect(clientID.c_str())) {
      // Aqui não precisa de subscribe, pois o ESP só vai ENVIAR dados
    } else { delay(5000); }
  }
}

void setup() {
  pinMode(2, INPUT_PULLUP); // Pino do botão (usando o resistor interno do ESP32)
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) { 
    reconnect(); 
  }
  client.loop();

  // Se o botão for pressionado (no INPUT_PULLUP o clique joga o pino para LOW)
  if (digitalRead(2) == LOW) {
    Serial.println("Botão pressionado! Enviando alerta...");
    
    // O comando mágico que envia a mensagem para a nuvem:
    client.publish("ifsdemg/sala/panico", "SOCORRO!"); 
    
    delay(1000); // Delay simples apenas para evitar que envie 1000 mensagens por segundo
  }
}