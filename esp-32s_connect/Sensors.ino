void baca_sensor(){
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
//Serial.print("Distance: ");
//Serial.println(distance);

//String message= "{\"eventName\":\"jarak\",\"status\":\"none\",\"panjang\":" + String(distance) + ",\"mac\":\"" + 1339782002177379 + "\"}";


String pesan;
pesan +="{\"eventName\":\"jarak\",\"status\":\"<none>\",\"panjang\":";
pesan += distance;
pesan += "\",\"v1\":\"1339782002177379\"}";

Serial.println(pesan);
char*msg=(char*)pesan.c_str();
publish_message(msg);
delay(2000);

}
