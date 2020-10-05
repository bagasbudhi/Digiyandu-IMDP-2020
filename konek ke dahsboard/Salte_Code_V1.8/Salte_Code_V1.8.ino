// PROGRAM SALTe ( MONITORING KADAR GARAM)
// DEVELOPER 
// ANGGA SETTIAWAN WAHYUDIN
// STHEVEN AIRLANGGGA
// RIFKI AKMAL MAULANA
// UNIVERSITAS SILIWANGI PRIDE :D

//LIBRARY YANG DIPAKAI
#include <Wire.h>
#include <DFRobot_SIM7000.h>

// PIN SIM7000e DFROBOT
#define PIN_TX     7
#define PIN_RX     8

//INISIALISASI
SoftwareSerial     mySerial(PIN_RX,PIN_TX);
DFRobot_SIM7000    sim7000;

//CONFIGURE
String Topic_Pub = "sim7000e/generic_device/v2/common";
String Username = "sim7000e-generic_device-v2_4732";
String Password = "1594202808_4732";
String MacAddress = "7620518671338958";

//Variable
String kirim;
void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
    sim7000.begin(mySerial);
    sim7000.turnOFF();
    delay(5000);

    Serial.println("Turn ON SIM7000......");
    if(sim7000.turnON()){                             //Turn ON SIM7000
        Serial.println("Turn ON !");
    }
    /*
    Serial.println("Set baud rate......");
    if(sim7000.setBaudRate(19200)){                                                            //Set baud rate from 115200 to 19200
        Serial.println("Set baud rate:19200");
    }else{
        Serial.println("Faile to set baud rate");
        while(1);
    }
    */
    mySerial.begin(19200);
    Serial.println("Initializing...");
    delay(3000);
    
  mySerial.println("AT");
  delay(3000);
  updateSerial();
  
  mySerial.println("AT+SMDISC");
  delay(3000);
  updateSerial();
  
  mySerial.println("AT+CREG?");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CGREG?");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CMEE=1");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CGACT?");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CIPSHUT");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CGATT=1"); //Set mode GPRS attached
  delay(3000);
  updateSerial();

  mySerial.println("AT+CSTT=\"internet\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CIICR");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CIFSR");
  delay(3000);
  updateSerial();
  
  mySerial.println("AT+CNACT=0");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CNACT=1,\"internet\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+CNACT?");
  delay(3000);
  updateSerial();

   mySerial.println("AT+SMCONF=\"URL\",\"mqtt.flexiot.xl.co.id\",\"1883\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+SMCONF=\"USERNAME\",\""+Username+"\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+SMCONF=\"PASSWORD\",\""+Password+"\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+SMCONF=\"CLIENTID\",\""+MacAddress+"\"");
  delay(3000);
  updateSerial();

  mySerial.println("AT+SMCONF=\"KEEPTIME\",60");
  delay(3000);
  updateSerial();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("==========");
   Serial.println("MENGIRIM");
   Sensor();
   delay(5000);
   mySerial.println("AT+SMCONN");
  delay(5000);
  updateSerial();

  mySerial.println("AT+SMSTATE?");
  delay(5000);
  updateSerial();
  
  mySerial.println("AT+SMPUB=\""+Topic_Pub+"\",512,1,1");
  delay(5000);
  updateSerial();  
  mySerial.println(kirim);
  delay(5000);
  updateSerial(); 
  kirim="";
   Serial.println("DONE");
    Serial.println("=========");
}

void Sensor()
{
kirim +="{\"eventName\":\"SALTe\",\"status\":\"<none>\",";
kirim +="\"Suhu\":\"";
kirim += random(0,100);
kirim += "\",\"Baume\":\"";
kirim += random(0,100);
kirim += "\",\"nacl\":\""; 
kirim += random(0,100);
kirim += "\",\"v2\":\""+MacAddress+"\"}";
}
void updateSerial()
{
  /*
   delay(500);
  while (Serial.available())
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while (mySerial.available())
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
  */
}
