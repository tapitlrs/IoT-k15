#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "IoT-Research"
#define WIFI_PASSWORD "Tapit168"

#define FIREBASE_HOST "iot-k15-c6170-default-rtdb.firebaseio.com"

/** The database secret is obsoleted, please use other authentication methods,
   see examples in the Authentications folder.
*/
#define FIREBASE_AUTH "BwHjox7nQlVdZLgSFfufro6N3pRMZcFCADAtBw6S"

//Define FirebaseESP8266 data object
FirebaseData fbdo;

FirebaseJson json;

void printResult(FirebaseData &data);
String path = "/IoT-k15/test";
boolean flag = false;
void ICACHE_RAM_ATTR daoLed() {
  digitalWrite(16, !digitalRead(16));
  flag = true;
  //  if(digitalRead(16)) {
  //    Firebase.setString(fbdo, path + "/status", "OFF");
  //  }
  //  else {
  //    Firebase.setString(fbdo, path + "/status", "ON");
  //  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);

  attachInterrupt(digitalPinToInterrupt(0), daoLed, FALLING);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  fbdo.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  fbdo.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);


  //
  //  json.clear().add("ledStatus", "ON");
  //  json.add("temperature", 25);

  //  Firebase.setJSON(fbdo, path, json);

  //  Firebase.setString(fbdo, path + "/string", "HELLO WORLD!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (flag) {
    flag = false;
    if (digitalRead(16)) {
      Firebase.setString(fbdo, path + "/status", "OFF");
    }
    else {
      Firebase.setString(fbdo, path + "/status", "ON");
    }
  }
}
