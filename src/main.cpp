#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <web.html>
#include <landing.svg>

const char *ssid = "Info Parkir";
const char *password = "infoparkir";
IPAddress ip(192, 168, 1, 1);
IPAddress netmask(255, 255, 255, 0);
WebServer server(80);

#define pin_masuk 1
#define pin_keluar 2
#define pin_park1 3
#define pin_park2 4
#define pin_park3 5
#define pin_gerbang 6

Servo myservo;
bool p1,p2,p3,m,k = false;

void BukaGerbang(){
  myservo.write(95);
}

void TutupGerbang(){
  myservo.write(0);
}

void SetupServo(){
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(13, 500, 2400);

}

void RunGerbang(){
  // Gerbang
  if((digitalRead(pin_masuk) == LOW ) or digitalRead(pin_keluar) == LOW){
    BukaGerbang();
  }else if(digitalRead(pin_masuk) and digitalRead(pin_keluar) == HIGH){
    TutupGerbang();
  }
}

void RunParkir(){

  // Slot parkit
  if(digitalRead(pin_park1) == LOW){
    p1 = true;
  }else{
    p1 = false;
  }

  if(digitalRead(pin_park2) == LOW){
    p2 = true;
  }else{
    p2 = false;
  }

  if(digitalRead(pin_park3) == LOW){
    p3 = true;
  }else{
    p3 = false;
  }

}

String Data;
void handleRoot(){
 server.send(200, "text/html", webpageCode);
}

void GetBG(){
  server.send(200, "text/plane", BG);
}
void SendData(){
 server.send(200, "text/plane", Data);
}

void setup() {
  Serial.begin(115200);
  
  SetupServo();
  myservo.write(0);
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(ip, ip, netmask);
  WiFi.softAP(ssid, password);

  server.on("/", handleRoot);
  server.on("/get", SendData);
  server.on("/getBG", GetBG);
  server.begin();

  pinMode(pin_masuk, INPUT);
  pinMode(pin_keluar, INPUT);
  pinMode(pin_park1, INPUT);
  pinMode(pin_park2, INPUT);
  pinMode(pin_park3, INPUT);
  Serial.println("Finish Stup");
}

void loop() {
  RunParkir();
  RunGerbang();
  Data += String(p1) + "," + String(p2) + "," + String(p3);
  server.handleClient();
  Data = "";
  delay(100);
}