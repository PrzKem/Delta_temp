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

void loop() {
  while (digitalRead(homeA) == HIGH)
  {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(dirPin, LOW);
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(homeA) == LOW)
  {
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(dirPin, HIGH);
    digitalWrite(pulPin, LOW);

  }
  delay(1000);
  for (int i = 0; i < 3440; i++)
  {
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(delayTime);
  }
  delay(1000);
  while (digitalRead(homeA) == HIGH)
  {
    digitalWrite(yellowLed, HIGH);
    digitalWrite(greenLed, LOW);
    digitalWrite(dirPin, LOW);
    digitalWrite(pulPin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pulPin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(homeA) == LOW)
  {
    digitalWrite(yellowLed, LOW);
    digitalWrite(greenLed, HIGH);
    digitalWrite(dirPin, HIGH);
    digitalWrite(pulPin, LOW);

  }
  while (1 == 1);
}
