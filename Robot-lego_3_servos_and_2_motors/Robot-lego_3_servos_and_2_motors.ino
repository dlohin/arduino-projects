#include <Servo.h>

Servo head;
Servo leftArm;
Servo rightArm;

const int motorRight = 12;
const int motorLeft = 11;

void setup() {
  // put your setup code here, to run once:

  head.attach(6);
  head.write(0);

  leftArm.attach(5);
  leftArm.write(0);

  rightArm.attach(3);
  rightArm.write(0);

  pinMode(motorRight,OUTPUT);
  pinMode(motorLeft,OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:

  head.write(800);
  leftArm.write(800);
  rightArm.write(400);
  digitalWrite(motorRight,LOW);
  digitalWrite(motorLeft,LOW);
  delay(2000);

  head.write(200);
  leftArm.write(1000);
  rightArm.write(100);
  digitalWrite(motorRight,HIGH);
  digitalWrite(motorLeft,LOW);
  delay(2000);

  digitalWrite(motorRight,LOW);
  digitalWrite(motorLeft,HIGH);
  head.write(800);
  delay(4000);
  rightArm.write(300);
  leftArm.write(1200);
  digitalWrite(motorRight,HIGH);
  digitalWrite(motorLeft,HIGH);

  delay(2000);
  head.write(200);
  digitalWrite(motorRight,LOW);
  digitalWrite(motorLeft,LOW);
  delay(1000);

  digitalWrite(motorRight,HIGH);
  digitalWrite(motorLeft,HIGH);

  delay(1000);
  digitalWrite(motorRight,HIGH);
  digitalWrite(motorLeft,LOW);

  delay(2000);
  digitalWrite(motorRight,LOW);
  digitalWrite(motorLeft,HIGH);
  delay(2000);
  head.write(300);
  leftArm.write(700);
  rightArm.write(400);

  delay(1000);
}
