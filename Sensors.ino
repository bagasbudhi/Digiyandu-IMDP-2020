void baca_sensor(){


for(a=0;a<100;a++){
  
distance = a;  

//String message= "{\"eventName\":\"jarak\",\"status\":\"none\",\"panjang\":" + String(distance) + ",\"mac\":\"" + 1339782002177379 + "\"}";

String pesan;
pesan +="{\"eventName\":\"jarak\",\"status\":\"none\",\"panjang\":";
pesan += distance;
pesan += "\",\"mac\":\"7130025626763309\"}";

Serial.println(pesan);
char*msg=(char*)pesan.c_str();
publish_message(msg);
delay(2000);
}
}
