/*
 * 
 * All the resources for this project: https://www.hackster.io/Aritro
 * Modified by Aritro Mukherjee
 * 
 * 
 */
 
#include <SPI.h>
#include <MFRC522.h> //RFID
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h> 
#include <TinyGPS.h> //include for the GPS handeling.
#include <IRremote.h>

int IR_RECEIVE_PIN = 7;
IRrecv irrecv(IR_RECEIVE_PIN);

TinyGPS gps;


 //For RFID
#define SS_PIN 10
#define RST_PIN 9
#define READ_PIN 2

//For IR remote
#define IR_PIN 7
 

//LCD I2C address
#define I2C_ADDRESS 0x27

//GPS Pins
#define GPS_RX 4
#define GPS_TX 3

// These would be saved in the EEPROM for your GeoCache
// For this example we will name them as const variables
const long TargetLat = 39217083;
const long TargetLon = -76868306;

struct //structure to organize our variables.
{
    float miles;
    float feet;
    float km;
} distance;


MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
LiquidCrystal_I2C lcd(I2C_ADDRESS,20,4);

/*
 * 
0 - 0xFF4AB5
1- 0xFF6897
2- 0xFF9867
3 - 0xFFB04F
4- 0xFF30CF
5- 0xFF18E7
6- 0xFF7A85
7- 0xFF10EF
8- 0xFF38C7
9- 0xFF5AA5
up - 0xFF629D
left - 0xFF22DD
right - 0xFFC23D
down -0xFFA857
ok - 0xFF02FD

* - 0xFF42BD
# - 0xFF42BD
 */

//Code to be printed in RFID Challenge
//Black lock
int code[5] = { 6, 7, 2, 7, 4 };
//Blue Lock 
int code2[5] = { 6, 4, 2, 8, 6 };

bool tags[10] = { false, false, false, false, false, false, false, false, false, false };

//0,8,4,2,9
long irCode[5] = { 16730805, 16726215, 16724175, 16750695, 16734885 };
int codeStep = 0;

//Code to get into the admin menu
//0,0,0,5,5
long adminIrCode[5] = { 0xFF4AB5, 0xFF4AB5, 0xFF4AB5, 0xFF18E7, 0xFF18E7 };
int adminStep = 0;

SoftwareSerial gpsSerial(GPS_RX,GPS_TX);


//Will track your overall progress through the game
int onStep=0;


 
void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication

  //Initiate RFID
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  
  Serial.println("Approximate your card to the reader...");
  Serial.println();

  //Initialize switches to test if there is a complete connection
  pinMode(READ_PIN, INPUT);

  //initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("hello everyone");
  lcd.setCursor(1,1);
  lcd.print("konichiwaa");
  delay(5);
  lcd.clear();

  irrecv.enableIRIn();  // Start the receiver

  Serial.print(("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);

  //Initiate GPS Serial . Need to do it later

}
void loop()
{

  checkForAdminCode();
  
  //RFID step
  if (onStep == 0) {
    rfidChallenge();
  }


  if(onStep == 1) {
    irChallenge();
  }

  if(onStep == 2){
     if (digitalRead(READ_PIN)) {
      onStep = 3;
      lcd.setCursor(0,3);
      lcd.print("Find the box .    ");
    }
  }


    if(onStep==3) {
      Serial.println("Start GPS");
      gpsChallenge();
    }
}

/*
#################################################
###############RFID Functions####################
#################################################
*/

//step 0 
int rfidChallenge() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);

     lcd.setCursor(1,0);
     
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
     //lcd.print(content);
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F7 3E 87 B2") 
  {
    lcd.setCursor(0,0);
    lcd.print(code[0]);
    tags[0] = true;
  } else if (content.substring(1) == "6A 1F 7D 80")  {
     lcd.setCursor(3,0);
     lcd.print(code[1]);
     tags[1] = true;
  } else if (content.substring(1) == "B7 BD 82 B2")  {
     lcd.setCursor(6,0);
     lcd.print(code[2]);
     tags[2] = true;
  } else if (content.substring(1) == "C7 F0 92 B3")  {
     lcd.setCursor(9,0);
     lcd.print(code[3]);
     tags[3] = true;
  } else if (content.substring(1) == "C7 2E 8D B3")  {
     lcd.setCursor(12,0);
     lcd.print(code[4]);
     tags[4] = true;
  } else if ( content.substring(1) == "17 0F 8A B3") {  // second row
     lcd.setCursor(0,1);
     lcd.print(code2[0]);
     tags[5] = true;
  } else if ( content.substring(1) == "0A E8 7A 67") {  // second row
     lcd.setCursor(3,1);
     lcd.print(code2[1]);
     tags[6] = true;
  } else if ( content.substring(1) == "C7 90 87 B2") {  // second row
     lcd.setCursor(6,1);
     lcd.print(code2[2]);
     tags[7] = true;
  } else if ( content.substring(1) == "AA ED 74 67") {  // second row
     lcd.setCursor(9,1);
     lcd.print(code2[3]);
     tags[8] = true;
  } else if ( content.substring(1) == "17 D9 88 B2") {  // second row
     lcd.setCursor(12,1);
     lcd.print(code2[4]);
     tags[9] = true;
  } else   {
    Serial.println(" Unknown Tag");
  }
  //IF you get all the tags.. move on to next step
  if(tags[0] && tags[1] && tags[2] && tags[3] && tags[4] && tags[5] && tags[6] && tags[7] && tags[8] && tags[9] ) {
    onStep = 1;
    lcd.setCursor(0,3);
    lcd.print("IR Challenge!");
    delay(1000);
  }
  
}


/*
#################################################
##################IR Challenge###################
#################################################
 */

void irChallenge() {
     decode_results results;        // Somewhere to store the results
    
    if (irrecv.decode(&results)) {  // Grab an IR code
        Serial.print("Got a code:");
        Serial.print(results.value);
        Serial.print(" Looking For: ");
        Serial.println(irCode[codeStep]);
        if (results.value != 0xffffffff)  //skip repeat signal
        {
           dumpCode(&results);           // Output the results as source code
           Serial.println("");           // Blank line between entries
 
          if(results.value == irCode[codeStep]) {
            Serial.println("Correct Value!");
            codeStep++;
          } else {
            Serial.println("Incorrect Value!");
            codeStep = 0;
          }
          //We completed the code
          if(codeStep == 5) {
            Serial.println("IR Challange completed!");
            lcd.setCursor(0,3);
            lcd.print("UP, DOWN, UP, DOWN");
            onStep=2;         
          }
        }
        delay(300);
        irrecv.resume();              // Prepare for the next value
    }
}


//+=============================================================================
// Display IR code
//
void ircode(decode_results *results) {
    // Panasonic has an Address
    if (results->decode_type == PANASONIC) {
        Serial.print(results->address, HEX);
        Serial.print(":");
    }

    // Print Code
    Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void encoding(decode_results *results) {
    switch (results->decode_type) {
    default:
    case UNKNOWN:
        Serial.print("UNKNOWN");
        break;
    case NEC:
        Serial.print("NEC");
        break;
    case SONY:
        Serial.print("SONY");
        break;
    case RC5:
        Serial.print("RC5");
        break;
    case RC6:
        Serial.print("RC6");
        break;
    case DISH:
        Serial.print("DISH");
        break;
    case SHARP:
        Serial.print("SHARP");
        break;
    case SHARP_ALT:
        Serial.print("SHARP_ALT");
        break;
    case JVC:
        Serial.print("JVC");
        break;
    case SANYO:
        Serial.print("SANYO");
        break;
    case MITSUBISHI:
        Serial.print("MITSUBISHI");
        break;
    case SAMSUNG:
        Serial.print("SAMSUNG");
        break;
    case LG:
        Serial.print("LG");
        break;
    case WHYNTER:
        Serial.print("WHYNTER");
        break;
    case AIWA_RC_T501:
        Serial.print("AIWA_RC_T501");
        break;
    case PANASONIC:
        Serial.print("PANASONIC");
        break;
    case DENON:
        Serial.print("Denon");
        break;
    case BOSEWAVE:
        Serial.print("BOSEWAVE");
        break;
    }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void dumpCode(decode_results *results) {
    // Start declaration
    Serial.print("unsigned int  ");          // variable type
    Serial.print("rawData[");                // array name
    Serial.print(results->rawlen - 1, DEC);  // array size
    Serial.print("] = {");                   // Start declaration

    // Dump data
    for (unsigned int i = 1; i < results->rawlen; i++) {
        Serial.print(results->rawbuf[i] * MICROS_PER_TICK, DEC);
        if (i < results->rawlen - 1)
            Serial.print(","); // ',' not needed on last one
        if (!(i & 1))
            Serial.print(" ");
    }

    // End declaration
    Serial.print("};");  //

    // Comment
    Serial.print("  // ");
    encoding(results);
    Serial.print(" ");
    ircode(results);

    // Newline
    Serial.println("");

    // Now dump "known" codes
    if (results->decode_type != UNKNOWN) {

        // Some protocols have an address
        if (results->decode_type == PANASONIC) {
            Serial.print("unsigned int  addr = 0x");
            Serial.print(results->address, HEX);
            Serial.println(";");
        }

        // All protocols have data
        Serial.print("unsigned int  data = 0x");
        Serial.print(results->value, HEX);
        Serial.println(";");
    }
}



/*
#################################################
###############GPS Functions#####################
#################################################
*/

void gpsChallenge() {
    gpsSerial.begin(9600);
    Serial.println(gpsSerial.available());
    
    if(isData()) {
        gpsFunc();
        getDistance(gps,TargetLat, TargetLon);
    }
    lcd.setCursor(0,1);
    
    lcd.print("Distance: ");
    lcd.print(distance.feet);
}

void gpsFunc() {
    Serial.println("in gpsfunc");
    if(isData())
    {
        getDistance(gps,TargetLat, TargetLon);
    }
    Serial.print("Miles: ");
    Serial.println(distance.miles);

    Serial.print("KM: ");
    Serial.println(distance.km);

    Serial.print("Feet: ");
    Serial.println(distance.feet);
}

bool isData() // feed the gps object with serial info and check if we are recieving a valid packet
{
  while (gpsSerial.available())
  {
    if (gps.encode(gpsSerial.read()))
      return true;
  }
  return false;
}

void getDistance(TinyGPS &gps, long targetLat, long targetLon)
{
  long lat, lon;
  unsigned long age;
  float latRad, lonRad;
  float flat, flon;
  float tlat, tlon;
  float tlatRad, tlonRad;
  float midLat, midLon;
  int samples=0;
  //----------------------------------------

  for (samples; samples< 20; samples++) //collect a few samples to make the data more accurate.Not sure if this is the best soloution.
  {
    gps.get_position(&lat, &lon, &age); //get the coords from the tinygps object
    if(age >= 5000) //if the data is old
    {
      delay(3000);//wait before we make another attempt
      isData(); //refresh the GPS object.
    }
    else
    {
        samples = 20;
    }
  }

  //convert to decimal degree
  flat = lat /100000.0;
  flon = lon /100000.0;
  lcd.setCursor(0,2);  
  Serial.print("lat: ");
  Serial.println(lat);
  lcd.setCursor(0,3);
  Serial.print("lon: ");
  Serial.println(lon);
  tlat = targetLat / 100000.0;
  tlon = targetLon / 100000.0;
  Serial.print("Tlat: ");
  Serial.println(tlat);
  Serial.print("tlon: ");
  Serial.println(tlon);
  //convert decimal degree into radian
  latRad = flat * 0.017453293;
  lonRad = flon * 0.017453293;
  tlatRad = tlat * 0.017453293;
  tlonRad = tlon * 0.017453293;

  midLat = tlatRad - latRad;
  midLon = tlonRad - lonRad;

  //Calculate the distance in KM
  float latSin = sin((latRad - tlatRad)/2);
  float lonSin = sin((lonRad - tlonRad)/2);
  distance.km = 2 * asin(sqrt((latSin*latSin) + cos(latRad) * cos(tlatRad) * (lonSin * lonSin)));
  distance.km = distance.km * 6371;



  //convert to miles
  distance.miles = distance.km * 0.621371192;
  distance.feet = distance.miles *5280;
  isData(); //refresh GPS during long calculations

  // --------------------------------

}


/*#############################################
 *##############Admin Menu#####################
 *###########################################
 */

 void checkForAdminCode() {
      decode_results results;
  
      if (irrecv.decode(&results)) {  // Grab an IR code
        Serial.print("Admin Got a code:");
        Serial.print(results.value,HEX);
        Serial.print(" Admin Looking For: ");
        Serial.println(adminIrCode[codeStep],HEX);
        if (results.value != 0xffffffff)  //skip repeat signal
        {
           dumpCode(&results);           // Output the results as source code
           Serial.println("");           // Blank line between entries
 
          if(results.value == adminIrCode[adminStep]) {
            Serial.println("Correct Value!");
            adminStep++;
          } else {
            Serial.println("Incorrect Value!");
            adminStep = 0;
          }
          //We completed the code
          if(adminStep == 5) {
            Serial.println("Entering Admin Menu!");
            delay(2000);
            adminMenu();
            adminStep = 0;
          }
        }
        delay(300);
        irrecv.resume();              // Prepare for the next value
    }
 }

 void adminMenu() {
    decode_results results;
    bool exitPlease = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Admin Menu");
    lcd.setCursor(0,1);
    lcd.print("1-GPS Functions");
    lcd.setCursor(0,3);
    lcd.print("3-Exit");
    while(exitPlease == false) {
      if (irrecv.decode(&results)) {  // Grab an IR code
        Serial.print("Got a code:");
        Serial.print(results.value);
        if (results.value != 0xffffffff) { //skip repeat signal
        
           if (results.value == 0xFF6897) {
              Serial.println("Enter GPS");
              irrecv.resume(); 
              gpsMenu();
              exitPlease =true;
           } else if (results.value == 0xFFB04F) {
              irrecv.resume(); 
              exitPlease = true;
           }
           
        }
        delay(300);
        irrecv.resume(); 
      }
      
   } //end while
}

void gpsMenu() {
    decode_results results;
    bool exitPlease = false;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GPS Menu");
    lcd.setCursor(0,1);
    lcd.print("1-Display GPS Coords");
    lcd.setCursor(0,3);
    lcd.print("3-Exit");
    while(exitPlease == false) {
      if (irrecv.decode(&results)) {  // Grab an IR code
        Serial.print("Got a code:");
        Serial.print(results.value);
        if (results.value != 0xffffffff) { //skip repeat signal
        
           if (results.value == 0xFF6897) {
              gpsSerial.begin(9600);
              irrecv.resume(); 
              Serial.println("in gpsfunc");
              printGpsCoords();
              exitPlease = true;       
           } else if (results.value == 0xFFB04F) {
              exitPlease = true;
           }
           
        }
        delay(300);
        irrecv.resume(); 
      }
      
   } //end while
}


void printGpsCoords() {
  lcd.clear();
  unsigned long age;
  
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  long lat, lon;
  unsigned long fix_age;

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Getting GPS Signal");
  
  
  do {
    // For one second we parse GPS data and report some key values
    for (unsigned long start = millis(); millis() - start < 1000;)
    {
      while (gpsSerial.available())
      {
        char c = gpsSerial.read();
        Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
          newData = true;
      }
    }
  
    if (newData)
    {
      gps.get_position(&lat, &lon, &fix_age);

      lcd.clear();
      Serial.print("LAT=");
      Serial.print(lat);
      lcd.setCursor(0,0);
      lcd.print("lat:");
      lcd.print(lat);
      Serial.print(" LON=");
      Serial.print(lon);
      lcd.setCursor(0,1);
      lcd.print("lon:");
      lcd.print(lon);
    }
    delay(5000);
  } while (newData != true);
}
 
