#include <WiFi.h>
#include <PubSubClient.h>
//pin sensor jarak
const int trigPin = 33;
const int echoPin = 32;
unsigned long duration;
unsigned long distance;
//
#define PUB_INTERVAL 5
unsigned long lastPublish = 0;

// Update these with values suitable for your network.
const char* ssid = "G6_6654";
const char* password = "87654321";

//Backend credentials
const char* mqtt_server = "mqtt.flexiot.xl.co.id";
String DEVICE_SERIAL = "1339782002177379" ; //update the device serial according to the serial given by the consumer portal 
const char* EVENT_TOPIC = "nodemcu/bagas_nodemcu/v1/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "/nodemcu/bagas_nodemcu/v1/sub";

const char* mqtt_username = "nodemcu-bagas_nodemcu-v1_4786";
const char* mqtt_password = "1595332629_4786";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[300];


void setup() {
  //sensor jarak
  pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

  // put your setup code here, to run once:
Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {
  // put your main code here, to run repeatedly:
if(WiFi.status() != WL_CONNECTED){
    setup_wifi();  
  }
  if (WiFi.status()== WL_CONNECTED && !client.connected()) {
    reconnect();
  }

  client.loop();

   if (millis() - lastPublish >= PUB_INTERVAL*1000UL){
    baca_sensor();
    lastPublish = millis();
  }
  
}
