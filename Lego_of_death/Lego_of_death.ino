#include <Servo.h>

int angle;
Servo arm;

void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  
  Serial.begin(9600);
  
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);

  arm.attach(2);
}

void loop() {
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  // put your main code here, to run repeatedly:

  arm.write(30);
  delay(2000);
  
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);  

  arm.write(2000);
  delay(2000);
}
