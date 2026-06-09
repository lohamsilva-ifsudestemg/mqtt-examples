#include <WiFi.h>
#include <PubSubClient.h>


// Credenciais para acesso a rede e ao MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";


// MUDANÇA: Usando o servidor público da HiveMQ, muito mais estável
const char* mqtt_server = "broker.hivemq.com";


// O tópico que você vai configurar no Switch do IoT MQTT Panel
const char* topico_led = "ifsdemg/sala/led";


WiFiClient IFSDEMG_Client;
PubSubClient client(IFSDEMG_Client);


// --- FUNÇÃO QUE CONVERTE OS BYTES EM TEXTO E CONTROLA O LED ---
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);


  // CONVERSÃO DE BYTES PARA TEXTO (STRING)
  String mensagem = "";
  for (int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.println("Conteúdo da mensagem: " + mensagem);


  // Ação para ligar ou desligar o LED no pino 2 diretamente
  if (mensagem == "1") {
    digitalWrite(2, HIGH);
    Serial.println("LED LIGADO");
  } else if (mensagem == "0") {
    digitalWrite(2, LOW);  
    Serial.println("LED DESLIGADO");
  }
}


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");


    // ID único e longo para evitar que o servidor nos derrube
    String clientID = "IFSDEMG_Prof_Unico_" + String(random(0, 999999));


    if (client.connect(clientID.c_str())) {
      Serial.println("connected");
     
      // O ESP diz ao broker que quer ouvir o tópico do LED
      client.subscribe(topico_led);
      Serial.println("Inscrito com sucesso no topico!");


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() {
  pinMode(2, OUTPUT); // Configura o pino 2 diretamente como saída
  digitalWrite(2, LOW); // Começa com o LED apagado
 
  Serial.begin(115200);
  setup_wifi();
 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); // Ativa a função de recebimento de mensagens
}


void loop() {
  // Se perder a conexão MQTT, ele tenta reconectar
  if (!client.connected()) {
    reconnect();
  }


  // MANTÉM A COMUNICAÇÃO ATIVA
  client.loop();
}
