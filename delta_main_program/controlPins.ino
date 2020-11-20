

void blynkLED(int ledPin, int delayTime, int times)
{
  int i = 0;
  boolean startStatus = digitalRead(ledPin);
  digitalWrite(ledPin, LOW);
  for (i = 0; i < times; i++)
  {
    digitalWrite(ledPin, HIGH);
    delay(delayTime);
    digitalWrite(ledPin, LOW);
    delay(delayTime);
  }
  digitalWrite(ledPin, startStatus);
}

void pinSetup()
{
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(A_HOME, INPUT_PULLUP);
  pinMode(B_HOME, INPUT_PULLUP);
  pinMode(C_HOME, INPUT_PULLUP);

  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}
