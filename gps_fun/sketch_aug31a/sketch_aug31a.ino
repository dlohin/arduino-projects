// get distance example
// using TinyGPS
// Written by Bill Heaster

#include <TinyGPS.h> //include for the GPS handeling.
TinyGPS gps;


// These would be saved in the EEPROM for your GeoCache
// For this example we will name them as const variables
const long TargetLat = 43164622;
const long TargetLon = 77609596;

struct //structure to organize our variables.
{
    float miles;
    float feet;
    float km;
} distance;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //start serial for the GPS module
}

void loop() {
  // put your main code here, to run repeatedly:
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
  while (Serial.available())
  {
    if (gps.encode(Serial.read()))
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
  tlat = targetLat / 100000.0;
  tlon = targetLon / 100000.0;

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
