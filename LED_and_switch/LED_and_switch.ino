const int switchPin = A3;
const int greenLed = A4;
const int redLed = A5;
const int yellowLed = A6;

void setup() {
  pinMode(greenLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);
  digitalWrite(redLed,HIGH);
}

void loop() {
 if(digitalRead(switchPin) == LOW)
 {
  digitalWrite(yellowLed, HIGH);
  digitalWrite(greenLed, LOW);
 }
 else
 {
  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, HIGH);
 }
}
