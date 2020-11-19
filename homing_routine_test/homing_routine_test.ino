const int homeA = A0;
const int yellowLed = A6;
const int greenLed = A4;
const int redLed = A5;

const int dirPin = 17;
const int pulPin = 16;
const int delayTime = 200;

void setup() {
  Serial.begin(9600);

  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(pulPin, OUTPUT);
  pinMode(homeA, INPUT_PULLUP);

  digitalWrite(yellowLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
  digitalWrite(dirPin, LOW);
  digitalWrite(pulPin, LOW);
}

void moveHome(int pul_pin, int dir_pin, int home_pin)
{
  while (digitalRead(home_pin) == HIGH)
  {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pul_pin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(home_pin) == LOW)
  {
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(dir_pin, HIGH);
    digitalWrite(pul_pin, LOW);
  }
  delay(1000);
  for (int i = 0; i < 3440; i++)
  {
    digitalWrite(pul_pin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pul_pin, LOW);
    delayMicroseconds(delayTime);
  }
  delay(1000);
  while (digitalRead(home_pin) == HIGH)
  {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pul_pin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(home_pin) == LOW)
  {
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, LOW);

  }
}

void blynkLED(int ledPin, int delayTime, int times)
{
  int i=0;
  boolean startStatus = digitalRead(ledPin);
  digitalWrite(ledPin,LOW);
  for(i=0;i<times;i++)
  {
    digitalWrite(ledPin,HIGH);
    delay(delayTime);
    digitalWrite(ledPin,LOW);
    delay(delayTime);
  }
  digitalWrite(ledPin, startStatus);
}

void loop() {
  moveHome(pulPin, dirPin, homeA);
  blynkLED(redLed, 200, 3);
  while (1 == 1);
}
