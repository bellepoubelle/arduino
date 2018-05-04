/*
This sensor is on the poubelle 1 (ID = 1)
www.bellepoubelle.fr
*/
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi.h>
#include <String.h>
#include <HttpClient.h>
int ledPin = 9;
//The ssid and the password of the network:
char ssid[] = "network"; 
char pass[] = "12345678";    
int keyIndex = 0;   
int status = WL_IDLE_STATUS;
char server[] = "www.bellepoubelle.fr"; 
WiFiClient client;
int level;
void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }
  pinMode(ledPin, OUTPUT);
  connectWifi();
  level = getLevel();
  Serial.println(level);
}
void loop() {
digitalWrite(ledPin, HIGH);
delay(500);
digitalWrite(ledPin, LOW);
delay(500);
// Read the output of the sensor (A0 is this case)
int analog = analogRead(A0);
//The levels we save are : 0%, 50%, 75%, 100%
if (analog <= 50 ){
  if ( level != 100)
  {
  Serial.println(level);
  connectWifi();
  changeLevel("100");
  level = 100;
  }
}
if (50 < analog){
  if(analog <= 800){
  if(level != 75){
   Serial.println(level);
  connectWifi();
  changeLevel("75");
  level = 75;
}}}
if (800 < analog){
  if (analog < 919){
  if(level != 50){
   Serial.println(level);
  connectWifi();
  changeLevel("50");
  level = 50;
}}}
if (920 < analog ){
    if(level != 0){
   Serial.println(level);
  connectWifi();
  changeLevel("0");
  level = 0;
    }
}
delay(5000);
}
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void connectWifi(){
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }
 
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);  
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();
 
  
}
// get the filling level the first time it connects
int getLevel(){
    Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    client.println("GET /rest/v1.0/poubelles/1 HTTP/1.1");
    client.println("HOST: api.belleboubelle.fr");
    client.println("Connection: close");
    client.println();
  }
  
  while(!client.available()) {
    ; 
  }
  String input = "";
  boolean b = false;
    while (client.available()) {
    char c = client.read();
    Serial.write(c);
    if (c == '{'){
      b = true;
    }
    if (b){
     input += c;
   }
}
    char y[input.length() + 1];
    strcpy(y, &input[0]);
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
  }
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(y);
  if (!root.success()) {
  Serial.println("no success");
  }
  int v = root["fillingLevel"];
  return v;
}
// modify the filling level
void changeLevel(String level){
     Serial.println("\nStarting connection to server...");
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    String req =  "GET 
https://api.bellepoubelle.fr/rest/v1.0/poubelles/1/fillingLevel/"+level+" 
HTTP/1.1";
    client.println(req);
    client.println("HOST: api.belleboubelle.fr");
    client.println("Connection: close");
    client.println();
  }
  
  while(!client.available()) {
    ; 
  }
    while (client.available()) {
    char c = client.read();
    Serial.write(c);
}
  
}

