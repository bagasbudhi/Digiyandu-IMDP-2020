#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define PUB_INTERVAL 30
unsigned lastPublish = 0;
const char* ssid = "masayu";
const char* password = "ayudhewayani";
//Backend credentials
const char* mqtt_server = "mqtt.flexiot.xl.co.id";
String DEVICE_SERIAL = "7620518671338958" ; //update the device serial according to the serial given by the consumer portal 
const char* EVENT_TOPIC = "sim7000e/generic_device/v2/common";
String SUB_TOPIC_STRING = "+/" + DEVICE_SERIAL + "//sim7000e/generic_device/v2/pub";
const char* mqtt_username = "nodemcu-bagas_nodemcu-v1_4786";
const char* mqtt_password = "1595332629_4786";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[300];

//TDS
#define analogInPin  A0  // Analog input pin 
int sensorValue; //adc value
float Salinitas;
float Konduktivitas;
//DS18B20
#include <OneWire.h>
int DS18S20_Pin = 2; //DS18S20 Signal pin on digital 2
OneWire ds(DS18S20_Pin);  // on digital pin 2
//Rumus
float Densitas;//Nilai Densitas
float Derajat_Baume;//Nilai Derajat_Bame
float NaCl; //Nilai %NaCl

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
 Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    setup_wifi();  
  }
  if (WiFi.status()== WL_CONNECTED && !client.connected()) {
    reconnect();
  }

  client.loop();

  if (millis() - lastPublish >= PUB_INTERVAL*1000UL){
    readSensor();
    lastPublish = millis();
  }
}
//DS18B20
  float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  byte present = ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad
  for (int i = 0; i < 9;  i++  ) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  return TemperatureSum;
}
 void publish_message(const char* message){
  client.publish(EVENT_TOPIC,message);  
} 
void readSensor(){
  sensorValue = analogRead(analogInPin);
  Konduktivitas = (0.2142*sensorValue)+494.93;
  Salinitas = (0.3417*sensorValue)+281.08;
  Serial.print("Konduktivitas: ");
  Serial.print(Konduktivitas); 
  Serial.println(" Siemens/meter");
  Serial.print("Salinitas: ");
  Serial.print(Salinitas); 
  Serial.println(" PPM ");
  delay(1000);
//DS18B20
  float temperature = getTemp();
  Serial.print("temperature: ");
  Serial.print(temperature); 
  Serial.println(" °C ");
  delay(1000); 
//Densitas
Densitas = ((999.84259+6.793952*0.01*temperature-9.09529*0.001*temperature*temperature+1.001685*0.0001*temperature*temperature*temperature-1.120083*0.000001*temperature*temperature*temperature*temperature+6.536332*0.000000001*temperature*temperature*temperature*temperature*temperature)
                 +(0.824493-4.0899*0.001*temperature+7.6438*0.00001*temperature*temperature-8.2467*0.0000001*temperature*temperature*temperature+5.3875*0.000000001*temperature*temperature*temperature*temperature)*Salinitas+(-5.72466*0.001+1.0227*0.0001*temperature-1.6546*0.000001*temperature*temperature)*Salinitas*0.5*Salinitas
                 +4.8314*0.0001*Salinitas*Salinitas);
//sensor calculation
  float Derajat_Baume = (145-145/Densitas);
  float NaCl = ((-0.1755*Derajat_Baume*Derajat_Baume)+(10.4555*Derajat_Baume)-(56.419));

String kirim;
kirim +="{\"eventName\":\"SALTe\",\"status\":\"<none>\",\"Suhu\":\"";
kirim += temperature;
kirim += "\",\"Baume\":\"";
kirim += Derajat_Baume;
kirim += "\",\"nacl\":\""; 
kirim += NaCl;
kirim += "\",\"v2\":\"7620518671338958\"}";
  Serial.println(kirim);
  char*msg = (char*)kirim.c_str();
  publish_message(msg);
  delay(2000);
}
  
  
  
  
  
  
  
  
 
