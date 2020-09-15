#include <LiquidCrystal.h>
#include <Wire.h>
#include <NewPing.h>

#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     9  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 400 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

#define WIN 6
#define LOSS 7
#define TIE 8

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
LiquidCrystal lcd(12,11, 5, 4, 3, 2);

int wins = 0;
int losses = 0;

int winState = 0;
int lossState = 0;
int tieState = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16,2);
  lcd.print("Welcome To Rock,");
  lcd.setCursor(0, 1);
  lcd.print("Paper, Scissors");

  pinMode(WIN, INPUT);
  pinMode(LOSS, INPUT);
  pinMode(TIE, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  unsigned int uS = sonar.ping();
  int distance = uS / US_ROUNDTRIP_CM;
  Serial.print(distance);
  Serial.println(" cm");

  //Sensing a hand
  if( distance <  20 ) {
    lcd.setCursor(0, 0);
    lcd.print("Shoot");
    lcd.setCursor(0, 1);
    lcd.print("                 ");
    delay(100);
    long hand = random(1, 3);

    //Make your choice
    if ( hand == 1) {
       clearScreen();
       lcd.print("Rock");
       delay(500);
    } else if (hand == 2){
       clearScreen();
       lcd.print("Paper");
       delay(500);
    } else {
       clearScreen();
       lcd.print("Scissor");
       delay(500);
    }
  }

  int winState = digitalRead(WIN);
  int lossState = digitalRead(LOSS);
  int tieState = digitalRead(TIE);
  
  Serial.print("Winstate: ");
  Serial.print(winState);
  if( winState == HIGH ) {
    wins++;
    printScore();
    delay(200);
  }
  if( lossState == HIGH ) {
    losses++;
    printScore();
    delay(200);
  }
  if( tieState == HIGH ) {
    printScore();
    delay(200);
  }
}

void printScore() {
lcd.setCursor(0, 0);
  lcd.print("Wins: ");
  lcd.print(wins);
  lcd.setCursor(0, 1);
  lcd.print("Losses: ");
  lcd.print(losses);
}

void clearScreen() {
       lcd.setCursor(0, 0);
       lcd.print("                 ");
       lcd.setCursor(0, 1);
       lcd.print("                 ");
       lcd.setCursor(0, 0);
}
