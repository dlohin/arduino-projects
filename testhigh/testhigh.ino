const int LEDPin = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(4, LOW);
  delay(400);
  digitalWrite(4, HIGH);
  delay(40);
}
