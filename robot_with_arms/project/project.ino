/*
   -- robot_with_arms --
   
   This source code of graphical user interface 
   has been generated automatically by RemoteXY editor.
   To compile this code using RemoteXY library 2.4.3 or later version 
   download by link http://remotexy.com/en/library/
   To connect using RemoteXY mobile app by link http://remotexy.com/en/download/                   
     - for ANDROID 4.5.1 or later version;
     - for iOS 1.4.1 or later version;
    
   This source code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.    
*/

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h> 

// RemoteXY connection settings  
#define REMOTEXY_SERIAL Serial 
#define REMOTEXY_SERIAL_SPEED 9600 
#define REMOTEXY_WIFI_SSID "RemoteXY" 
#define REMOTEXY_WIFI_PASSWORD "12345678" 
#define REMOTEXY_SERVER_PORT 6377 


#include <RemoteXY.h>
#include <Servo.h>

Servo head;
Servo leftArm;
Servo rightArm;

const int motorRight = 12;
const int motorLeft = 11;

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,6,0,0,0,58,0,10,29,0,
  1,0,23,36,12,12,94,31,103,111,
  0,1,0,6,49,12,12,205,31,108,
  0,4,160,-2,3,37,9,6,26,4,
  160,67,4,35,9,36,26,1,0,39,
  51,12,12,37,31,114,0,4,160,32,
  4,34,8,95,26 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t forward; // =1 if button pressed, else =0 
  uint8_t left; // =1 if button pressed, else =0 
  int8_t l_arm; // =-100..100 slider position 
  int8_t r_arm; // =-100..100 slider position 
  uint8_t right; // =1 if button pressed, else =0 
  int8_t head; // =-100..100 slider position 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////



void setup() 
{
  RemoteXY_Init (); 
  
  head.attach(6);
  head.write(0);

  leftArm.attach(5);
  leftArm.write(0);

  rightArm.attach(3);
  rightArm.write(0);
  
  leftArm.write(800);
  rightArm.write(400);

  pinMode(motorRight,OUTPUT);
  pinMode(motorLeft,OUTPUT);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 
  
  //move the right wheel to go left
  
  if(RemoteXY.forward==1) {
      digitalWrite(motorRight,HIGH);
      digitalWrite(motorLeft,HIGH);
  } else if ( RemoteXY.left==1) {
    digitalWrite(motorRight,HIGH);
    digitalWrite(motorLeft,LOW);
  } else if ( RemoteXY.right==1) {
    digitalWrite(motorRight,LOW);
    digitalWrite(motorLeft,HIGH);
  } else {
    digitalWrite(motorRight,LOW);
    digitalWrite(motorLeft,LOW);
  }

}
