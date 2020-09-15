/*
   -- New project --
   
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
#define REMOTEXY_MODE__HARDSERIAL

#include <RemoteXY.h>
#include <Servo.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 9600

#define PIN_LIGHT 8
#define PIN_TWIST 5
#define PIN_HEAD 6


Servo twist;
Servo head;

int pos_twist;
int pos_head;

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,3,0,0,0,29,0,10,13,0,
  4,0,5,8,14,48,2,26,4,128,
  22,44,69,14,1,26,1,0,41,9,
  27,27,6,31,88,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  int8_t head; // =0..100 slider position 
  int8_t twist; // =0..100 slider position 
  uint8_t light; // =1 if button pressed, else =0 

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
  pinMode (PIN_LIGHT, OUTPUT);
  twist.attach(PIN_TWIST);
  twist.write(600);
  head.attach(PIN_HEAD);
  head.write(600);
  
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  digitalWrite(PIN_LIGHT, (RemoteXY.light==0)?LOW:HIGH);

  pos_twist=map(RemoteXY.twist,-100,100,0,1024);
  twist.write(pos_twist);
  pos_head=map(RemoteXY.head,-100,100,500,920);
  head.write(pos_head);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay() 


}
