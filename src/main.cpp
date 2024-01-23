#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <web.html>

const char *ssid = "Info Parkir";
const char *password = "infoparkir";
IPAddress ip(192, 168, 1, 1);
IPAddress netmask(255, 255, 255, 0);
WebServer server(80);

#define pin_park1 3
#define pin_park2 4
#define pin_park3 5
#define pin_masuk 6
#define pin_keluar 7

Servo myservo;
bool p1,p2,p3,m,k = false;
int suspend_gerbang = 0;

void BukaGerbang(){
  myservo.write(90);
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

bool cekPenuh(){
  if((p1 && p2 && p3) == true){
    return true;
  }else{
    return false;
  }
}

void RunGerbang(){
  // Gerbang
  if(cekPenuh() == false){
    if(
        (digitalRead(pin_masuk) == LOW ) or
        digitalRead(pin_keluar) == LOW){
          BukaGerbang();
          suspend_gerbang = 0;
    }else if(
        digitalRead(pin_masuk) and
        digitalRead(pin_keluar) == HIGH){
          if(suspend_gerbang <= 3000){
            suspend_gerbang += 200;
      }else if(suspend_gerbang >= 2000){
        TutupGerbang();
        suspend_gerbang = 0;
      }else{
        suspend_gerbang = 0;
      }
    }
  }else{
    myservo.write(0);
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