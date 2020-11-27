struct waitParams
{
  long actualTime;
  long lastTime;
  long delayTime;
};

waitParams manualDelay;

void automaticState()
{
  int maxReq = getMetaInfo();
  int i = 1, actualStatus = 0, j = 0;;
  float x, y, z;
  float theta[3] = {0.0};
  const int delayTime = 1000;

  for (i = 1; i <= maxReq; i++)
  {
    requestValues(i, &x, &y, &z, &j);
    if (j == 1)
    {
      actualStatus = delta_calcInverse(x, y, z, theta[0], theta[1], theta[2]);
      if (DEBUG)
      {
        Serial.print("\nStatus after calculus: ");
        Serial.println(actualStatus);
      }
      actualStatus = checkPos(actualStatus, x, y, z, theta[0], theta[1], theta[2]);

      if (actualStatus < 0)
      {
        if (DEBUG)
        {
          Serial.println("Error!");
          Serial.print("Actual status: ");
          Serial.println(actualStatus);
          Serial.print("Theta: [");
          Serial.print(theta[0]); Serial.print(", ");
          Serial.print(theta[1]); Serial.print(", ");
          Serial.print(theta[2]); Serial.println("]");
          Serial.print("Pos: [");
          Serial.print(x); Serial.print(", ");
          Serial.print(y); Serial.print(", ");
          Serial.print(z); Serial.println("]");

        }
        while (1 == 1)
        {
          blynkLED(RED_LED, 100, actualStatus);
          delay(1000);
        }
      }
      else
      {
        goToPos(theta[0], theta[1], theta[2]);
        delay(delayTime);
      }
    }
    else if (j == 2)
    {
      delay((int)x);
    }

  }
}

void manualState()
{
  int actualStatus = 0;
  int i = 0;
  float x, y, z;
  float theta[3];
  manualDelay.actualTime = millis();
  if (manualDelay.actualTime >= manualDelay.lastTime + manualDelay.delayTime)
  {
    requestRealTime(&i, &x, &y, &z);
    manualDelay.actualTime = manualDelay.lastTime;
    if (i == 1)
    {
      actualStatus = delta_calcInverse(x, y, z, theta[0], theta[1], theta[2]);
      if (DEBUG)
      {
        Serial.print("\nStatus after calculus: ");
        Serial.println(actualStatus);
      }
      actualStatus = checkPos(actualStatus, x, y, z, theta[0], theta[1], theta[2]);

      if (actualStatus < 0)
      {
        if (DEBUG)
        {
          Serial.println("Error!");
          Serial.print("Actual status: ");
          Serial.println(actualStatus);
          Serial.print("Theta: [");
          Serial.print(theta[0]); Serial.print(", ");
          Serial.print(theta[1]); Serial.print(", ");
          Serial.print(theta[2]); Serial.println("]");
          Serial.print("Pos: [");
          Serial.print(x); Serial.print(", ");
          Serial.print(y); Serial.print(", ");
          Serial.print(z); Serial.println("]");

        }
        while (1 == 1)
        {
          blynkLED(RED_LED, 100, actualStatus);
          delay(1000);
        }
      }
      else
      {
        goToPos(theta[0], theta[1], theta[2]);
        delay(delayTime);
      }
    }
  }


}

void stateSetup()
{
  manualDelay.delayTime = 500;
  manualDelay.actualTime = millis();
  manualDelay.lastTime = manualDelay.actualTime;
}
