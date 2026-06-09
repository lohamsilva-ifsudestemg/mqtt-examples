#include <WiFi.h>
#include <PubSubClient.h>

// Credenciais para acesso a rede e ao MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";
WiFiClient IFSDEMG_Client;
PubSubClient client( IFSDEMG_Client);

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect


    if (client.connect("IFSDEMG_Client")) {
      Serial.println("connected");


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);


    }
  }
}


void conectado_wifi(){
  if (WiFi.status()){
    digitalWrite(2, HIGH);
    delay(1000);
  } else {
  digitalWrite(2, LOW);
  delay(1000);
  }
}


void conectado_mosquitto(){
  if (client.connected()){
    digitalWrite(15, HIGH);
    delay(1000);
  } else {
    digitalWrite(15, LOW);
    delay(1000);
  }  }

void setup() {
  pinMode(2, OUTPUT);  
  pinMode(15, OUTPUT);  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  conectado_wifi();
  conectado_mosquitto();
}