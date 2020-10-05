#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <DHT.h>

//Connecting WIFI
const char *ssid = "Sukses 72";
const char *pass = "kemaren6";

//API Flex IoT
const char* mqtt_server = "mqtt.flexiot.xl.co.id";
String DEVICE_SERIAL = "6726554719793875" ; //update the device serial according to the serial given by the consumer portal 
const char* EVENT_TOPIC = "wemos/autodox_prototype/v1/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "/wemos/autodox_prototype/v1/sub";
const char* mqtt_username = "wemos-autodox_prototype-v1_4697";
const char* mqtt_password = "1593689638_4697";

//Inisialisasi
#define PUB_INTERVAL 30
#define DHTPIN D5
#define TRIG_PIN D8
#define ECHO_PIN D7
#define LED D0
#define buzzer D4
#define IRSensor D1

char msg[250];
int pos = 0;
unsigned long lastPublish = 0;

Servo myservo;
DHT dht(DHTPIN, DHT11);

WiFiClient wificlient;
PubSubClient client(wificlient);

void setup_wifi() 
{

       Serial.println();
       Serial.print("Connecting to ");
       Serial.println(ssid);

       WiFi.begin(ssid, pass);

       while (WiFi.status() != WL_CONNECTED)
       {
        delay(100);
        Serial.print("...");
       }
       randomSeed(micros());

       Serial.println("");
       Serial.println("");
       Serial.print("WiFi connected :");
       Serial.println(ssid);
       Serial.print("IP address: ");
       Serial.println(WiFi.localIP());     
 }

 void callback(char* topic, byte* payload, long length)
{
  Serial.print("Message Arrived [");
  Serial.print(SUB_TOPIC_STRING);
  Serial.print("]");
  for (int i=0; i < length; i++)
  {
    msg[i] = (char)payload[i]; 
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
      delay(500);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  delay(1000);
  pinMode (IRSensor, INPUT); // sensor pin INPUT
  pinMode (LED, OUTPUT); // Led pin OUTPUT
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(buzzer, OUTPUT);
  myservo.attach(D2);
  dht.begin();
  
}

void loop() 
{ 
  if(WiFi.status() != WL_CONNECTED){
    setup_wifi();  
  }
  if (WiFi.status()== WL_CONNECTED && !client.connected()) {
    reconnect();
  }
  client.loop();
  readIRSensor();
  if (millis() - lastPublish >= PUB_INTERVAL*1000UL){
    readSensor();
    readUSSensor();
    lastPublish = millis();
  }
}

void publish_message(const char* message)
{
  client.publish(EVENT_TOPIC,message);  
}

void readSensor()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      String message = "{\"eventName\":\"DHT\",\"status\":\"none\",\"humid\":"+ String(h)+ ",\"temp\":" + String(t)+ ",\"mac\":\"6726554719793875\"}";
    //String message = "{\"eventName\":\"DHT\",\"status\":\"none\",\"humid\":"+ String(h)+ ",\"temp\":" + String(t)+ ",\"mac":"\"8141224079121181\"}";
      Serial.println(message);
      Serial.print("\n");
      char* msg = (char*)message.c_str();
      publish_message(msg);
      delay(100);
}

void readUSSensor()
{
  //Ultrasonic
      digitalWrite(TRIG_PIN, LOW);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, HIGH);
      delayMicroseconds(2);
      digitalWrite(TRIG_PIN, LOW);

      const unsigned long duration = pulseIn(ECHO_PIN, HIGH);
      float distance = ((duration/2)/29.1);
      float max_distance = 12.5 ;
      distance = max_distance - distance;
      distance = (distance/max_distance)*100;
       if (duration == 0)
      {
        Serial.println("Tidak Ada Halangan");
      }
      else 
      {
        Serial.print("Jarak Ke Objek : ");
        Serial.print(distance);
        Serial.println(" CM");
      }

      //Display Flex IoT
      //String message = "{\"eventName\":\"DHT\",\"status\":\"none\",\"humid\":"+ String(h)+ ",\"temp\":" + String(t)+ ",\"mac\":\"6726554719793875\"}";
      String message = "{\"eventName\":\"Volume\",\"status\":\"none\",\"volum\":" + String (distance) + ",\"mac\":\"6726554719793875\"}";
      Serial.println(message);
      Serial.print("\n");
      char* msg = (char*)message.c_str();
      publish_message(msg);
}

void readIRSensor()
{
  int statusSensor = digitalRead (IRSensor);
    if (statusSensor == 1)
    {
      digitalWrite(LED, LOW); // LED LOW
    }
    else
    {
      digitalWrite(LED, HIGH); // LED High
      Serial.println("Lama Nyala");
      for (pos = 180; pos >= 0; pos -= 1)
       
      { 
        myservo.write(pos);              
        delay(1);                       
      }
      Serial.println("Sabun Keluar");
       for (pos = 0; pos <= 180; pos += 1)
      { 
        myservo.write(pos);              
        delay(1);                       
      }
    }
    delay(1000);
}
