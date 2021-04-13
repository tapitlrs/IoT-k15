#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "IoT-Research";
const char* password = "Tapit168";
const char* mqtt_server = "192.168.1.2";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

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

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char json[200];
  for (int i = 0; i < length; i++) {
    json[i] = (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);
  const char* state = doc["status"];
  Serial.println(state);

  if (String(state) == "on") {
    digitalWrite(BUILTIN_LED, LOW);
  }
  else if (String(state) == "off") {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("TenHocVien/Light/Event");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void ICACHE_RAM_ATTR handleInterrupt() {
  StaticJsonDocument<200> doc;
  char output[200];
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
  if (digitalRead(BUILTIN_LED) == LOW ) {
    doc["status"] = "on";
    serializeJson(doc, output);
  }
  else {
    doc["status"] = "off";
    serializeJson(doc, output);
  }
  client.publish("outTopic", output);
}


void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  attachInterrupt(digitalPinToInterrupt(0), handleInterrupt, FALLING);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
