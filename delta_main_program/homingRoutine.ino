#define STEPS 3440

const int delayTime = 200;

void przyspieszanie(const int pulPin, const int dirPin, boolean dir)
{
  int max_speed = 50;
  float c0 = 10000;
  float d;

  digitalWrite(dirPin, dir);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);

  for (int i = 0; i < STEPS; )
  {
    if (i < STEPS / 2)
    {
      d = c0 * (sqrt(i + 1) - sqrt(i));
    }
    else
    {
      d = c0 * (sqrt((STEPS) - i + 1) - sqrt((STEPS) - i));
    }
    if (d < max_speed)
      d = max_speed; //zabezpieczenie zeby sie nie rozpedzało w nieskonczonosc

    if (canMove)
    {
      digitalWrite(pulPin, HIGH);
      delayMicroseconds(d);
      digitalWrite(pulPin, LOW);
      delayMicroseconds(d);
      i++;
    }
    else
    {
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      blynkLED(RED_LED, 3, 1000);
    }
  }
}

void moveToSensor(int pul_pin, int dir_pin, int home_pin)
{
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(dir_pin, LOW);
  while (digitalRead(home_pin) == HIGH)
  {
    if (canMove)
    {
      digitalWrite(pul_pin, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(pul_pin, LOW);
      delayMicroseconds(delayTime);
    }
    else
    {
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, LOW);
      blynkLED(RED_LED, 3, 1000);
    }
  }
}

void moveHome(int pul_pin, int dir_pin, int home_pin)
{
  moveToSensor(pul_pin, dir_pin, home_pin);
  blynkLED(GREEN_LED, 3, 100);
  delay(1000);
  przyspieszanie(pul_pin, dir_pin, HIGH);
  blynkLED(GREEN_LED, 3, 100);
  delay(1000);
  moveToSensor(pul_pin, dir_pin, home_pin);
  blynkLED(GREEN_LED, 3, 100);
  delay(1000);

}

void homingRoutine()
{
  int i = 0;
  moveHome(A_PUL_PIN, A_DIR_PIN, A_HOME);
  blynkLED(RED_LED, 200, 3);
  if (DEBUG)
    Serial.println("\nA in home position");
  moveHome(B_PUL_PIN, B_DIR_PIN, B_HOME);
  blynkLED(RED_LED, 200, 3);
  if (DEBUG)
    Serial.println("B in home position");
  moveHome(C_PUL_PIN, C_DIR_PIN, C_HOME);
  blynkLED(RED_LED, 200, 3);
  if (DEBUG)
    Serial.println("C in home position");
  for (i = 0; i < 3; i++)
    prevPos[i] = -35.22;

  goToPos(0, 0, 0);
  delay(1000);
  if (DEBUG)
    Serial.println("Homing done\n");
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}
