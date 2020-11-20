const int delayTime = 200;

void moveHome(int pul_pin, int dir_pin, int home_pin)
{
  while (digitalRead(home_pin) == HIGH)
  {
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pul_pin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(home_pin) == LOW)
  {
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
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
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, HIGH);
    delayMicroseconds(delayTime);
    digitalWrite(pul_pin, LOW);
    delayMicroseconds(delayTime);
  }
  if (digitalRead(home_pin) == LOW)
  {
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(dir_pin, LOW);
    digitalWrite(pul_pin, LOW);

  }
}
