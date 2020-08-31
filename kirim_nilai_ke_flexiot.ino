#include <WiFi.h>
#include <PubSubClient.h>

//variabel
int distance = 0;
int a=0;
//
#define PUB_INTERVAL 5
unsigned long lastPublish = 0;

// Update these with values suitable for your network.
const char* ssid = "Masayu";//"G6_6654";
const char* password = "ayudhewayani";//"87654321";

//Backend credentials
const char* mqtt_server = "mqtt.flexiot.xl.co.id";
String DEVICE_SERIAL = "7130025626763309" ; //update the device serial according to the serial given by the consumer portal 
const char* EVENT_TOPIC = "nodemcu/bagas_nodemcu/v2/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "/nodemcu/bagas_nodemcu/v2/sub";

//const char* mqtt_username = "nodemcu-bagas_nodemcu-v172034_4788";
//const char* mqtt_password = "1595337276_4788";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[300];


void setup() {
 

  // put your setup code here, to run once:
Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);
  delay(1000);
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
