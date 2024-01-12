#include <Arduino.h>
#include <ESP32Servo.h>

#define pin_masuk 1
#define pin_keluar 2
#define pin_park1 3
#define pin_park2 4
#define pin_park3 5
#define pin_gerbang 6

Servo myservo;
bool p1,p2,p3,m,k = false;

void BukaGerbang(){
  myservo.write(90);
}

void TutupGerbang(){
  myservo.write(0);
}

void SetupServo(){
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(13, 500, 2400);

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

  // Gerbang
  if(digitalRead(pin_masuk) == LOW){
    BukaGerbang();
    m = true;
  }else{
    TutupGerbang();
    m = false;
  }

  if(digitalRead(pin_keluar) == LOW){
    k = true;
  }else{
    k = false;
  }

}

void setup() {
  Serial.begin(115200);
  SetupServo();
  myservo.write(0);
  pinMode(pin_masuk, INPUT);
  pinMode(pin_keluar, INPUT);
  pinMode(pin_park1, INPUT);
  pinMode(pin_park2, INPUT);
  pinMode(pin_park3, INPUT);
}

void loop() {
  RunParkir();
  Serial.print(p1);
  Serial.print(p2);
  Serial.print(m);
  Serial.println(p3);
  delay(500);
}