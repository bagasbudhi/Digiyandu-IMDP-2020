/* Program device prototype Digiyandu IMDP 2020 

By: Bagas Budhi Permana | Bagaspermana88@gmail.com

*/


/*
GENERIC SDK FOR ESP
Add ESP8266 and PubSubClient libraries into your arduino libraries forlder
You can find those library files inside the zip file.
update the send_event function and do_actions function with respect your requirements.
*/
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "DHT.h"

#include <HX711_ADC.h> // library Timbangan
#include <EEPROM.h>

//suhu
#include <Wire.h>
#include <Adafruit_MLX90614.h> //library suhu sensor MLX90614

//pin sensor berat
const int HX711_dout = 19; //mcu > HX711 dout pin
const int HX711_sck = 18; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

double hasil;
double hasil_fix;
double bacaan_suhu;
const int calVal_calVal_eepromAdress = 0;
long t;


#define DHTPIN 11     // what digital pin we're connected to
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);
#define PUB_INTERVAL 5

unsigned long lastPublish = 0;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

//variable sensor jarak
const int trigPin = 5;
const int echoPin = 4;

// defines variables
long duration;
int distance;

// Update these with values suitable for your network.
const char* ssid = "Resta Maharani"; // isi dengan nama WiFi
const char* password = "babylion1709"; // isi dengan password WiFi

//Backend credentials --- liat di account Flexiot
/*
Account Flexiot Bagas
*/
const char* mqtt_server = "mqtt.flexiot.xl.co.id";
String DEVICE_SERIAL = "7360319863815522" ; //update the device serial according to the serial given by the consumer portal 
const char* EVENT_TOPIC = "nodemcu/bagas_nodemcu/v5/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "/nodemcu/bagas_nodemcu/v5/sub";
const char* mqtt_username = "nodemcu-bagas_nodemcu-v5_5022";
const char* mqtt_password = "1600483415_5022";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[300];

void do_actions(const char* message);
void readSensor();

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

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

//receiving a message
void callback(char* topic, byte* payload,long length) {
  Serial.print("Message arrived [");
  Serial.print(SUB_TOPIC_STRING);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  do_actions(msg);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),mqtt_username,mqtt_password)) {
      Serial.println("connected");
      //subscribe to the topic
      const char* SUB_TOPIC = SUB_TOPIC_STRING.c_str();
      client.subscribe(SUB_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup() {
  Serial.begin(115200);
  //setup_wifi(); // uncomment kalo mau offline
  client.setServer(mqtt_server, 1883);
  
  //client.setCallback(callback);
  //jarak
  setup_jarak();

  //sensor berat
  setup_berat();
 
  Serial.println(F("===========|| DIGIYANDU IMDP 2020 ||=========="));
  delay(1000);
  dht.begin();
  //suhu
  mlx.begin(); 

}

void loop() {

 /* if(WiFi.status() != WL_CONNECTED){
    setup_wifi();  
  }
  if (WiFi.status()== WL_CONNECTED && !client.connected()) {
    reconnect();
  } 

  client.loop(); // uncomment kalo mau offline

  if (millis() - lastPublish >= PUB_INTERVAL * 1000UL) {
    readSensor();
    lastPublish = millis();
  }
*/
  //jarak
  jarak();
  loop_berat();
}

void publish_message(const char* message){
  client.publish(EVENT_TOPIC,message);  
}

//====================================================================================================================================================================  
void send_event(){
  //create this function to send some events to the backend. You should create a message format like this
  /* Eg :{
        "mac":"6655591876092787",
        "eventName":"eventOne",
        "state":"none",
        "eventOne":{
          "ev1Value1":30
        }
      }
  
  */
  //Should call publish_message() function to send events. Pass your message as parameter
  // Eg : publish_message(message);
}
//====================================================================================================================================================================

//==================================================================================================================================================================== 
void do_actions(const char* message){
  //Create this function according to your actions. you will receive a message something like this
  /* Eg : {
        "action":"actionOne",
        "param":{
          "ac1Value1":"1110" ,
          "parentMac":"6655591876092787",
          "ac1Value4":"port:03",
          "ac1Value5":"on",
          "mac":"6655591876092787",
          "ac1Value2":"2220",
          "ac1Value3":"567776"
        }
      } */
  
  Serial.println(message);
}
//=====================================================================================================================================================================

void readSensor() {
  //sensor calculation
  float t = mlx.readObjectTempC();//33;//dht.readTemperature();
  float h = 45;//dht.readHumidity(); //tidak dipakai
  float w = hasil_fix;
  float he = distance;
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

//FlexIoT JSON format --> {"eventName":"SensorDHT","status":"<none>","temp":"<Temperature>","humid":"<Humidity>","mac":"<macAddressOfDevice>"}

//{"eventName":"SensorDHT","status":"<none>","temp":"<Temperature>","humid":"<Humidity>","weight":"<Weight>","Height":"<Height>","mac":"<macAddressOfDevice>"}

  String message = "{\"eventName\":\"SensorDHT\",\"status\":\"none\",\"temp\":" + String(t) + ",\"humid\":" + String(h) + ",\"weight\":" + String(w) + ",\"Height\":" + String(he) + ",\"mac\":\"" + DEVICE_SERIAL + "\"}";
  Serial.println(message);
  char* msg = (char*)message.c_str();
  publish_message(msg); // send the event to backend

//  Serial.print(F("Humidity: "));
//  Serial.print(h);
 // Serial.print(F("%  Temperature: "));
//  Serial.print(t);
//  Serial.print(F("°C "));

  
}
