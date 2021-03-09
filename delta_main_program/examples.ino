void circle(int delayTime)
{
  float circle[CIRCLE_ARRAY_SIZE][3];
  float theta[3];
  int actualStatus = 0, i = 0;

  for (i = 0; i < CIRCLE_ARRAY_SIZE / 2; i++)
  {
    circle[i][0] = -CIRCLE_RADIUS + (CIRCLE_RADIUS * 2 / (float)CIRCLE_ARRAY_SIZE) * i;
    circle[i][1] = sqrt(CIRCLE_RADIUS * CIRCLE_RADIUS - (circle[i][0] * circle[i][0]));
    circle[i][2] = -300;
  }
  for (i = 0; i < CIRCLE_ARRAY_SIZE / 2; i++)
  {
    circle[CIRCLE_ARRAY_SIZE - 1 - i][0] = (2 * CIRCLE_RADIUS / (float)CIRCLE_ARRAY_SIZE) * i - CIRCLE_RADIUS;
    circle[CIRCLE_ARRAY_SIZE - 1 - i][1] = (-1) * sqrt(CIRCLE_RADIUS * CIRCLE_RADIUS - (circle[CIRCLE_ARRAY_SIZE - 1 - i][0] * circle[CIRCLE_ARRAY_SIZE - 1 - i][0]));
    circle[CIRCLE_ARRAY_SIZE - 1 - i][2] = -300;
  }

  for (i = 0; i < CIRCLE_ARRAY_SIZE; i++)
  {
    actualStatus = delta_calcInverse(circle[i][0], circle[i][1], circle[i][2], theta[0], theta[1], theta[2]);

    if (DEBUG)
    {
      Serial.print("Actual status: ");
      Serial.println(actualStatus);
    }
    //actualStatus = checkPos(actualStatus,circle[i][0], circle[i][1], circle[i][2], theta[0], theta[1], theta[2]);

    if (actualStatus < 0)
    {
      if (DEBUG)
      {
        Serial.println("Error!");
        Serial.print("Theta: [");
        Serial.print(theta[0]); Serial.print(", ");
        Serial.print(theta[1]); Serial.print(", ");
        Serial.print(theta[2]); Serial.println("]");
        Serial.print("Pos: [");
        Serial.print(circle[i][0]); Serial.print(", ");
        Serial.print(circle[i][1]); Serial.print(", ");
        Serial.print(circle[i][2]); Serial.println("]");

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
  if (DEBUG)
    Serial.println("Circle done");
}

void cube(int delayTime)
{
  float theta[3];
  float cube[8][3];
  int actualStatus = 0, i = 0;
  const float maxPos = 20;
  const float minZ = -300;
  const float maxZ = -250;

  cube[0][0] =  maxPos; cube[0][1] = -maxPos; cube[0][2] = minZ; //A
  cube[1][0] =  maxPos; cube[1][1] =  maxPos; cube[1][2] = minZ; //B
  cube[2][0] =  maxPos; cube[2][1] =  maxPos; cube[2][2] = maxZ; //C
  cube[3][0] =  maxPos; cube[3][1] = -maxPos; cube[3][2] = maxZ; //D
  cube[4][0] = -maxPos; cube[4][1] = -maxPos; cube[4][2] = minZ; //A'
  cube[5][0] = -maxPos; cube[5][1] =  maxPos; cube[5][2] = minZ; //B'
  cube[6][0] = -maxPos; cube[6][1] =  maxPos; cube[6][2] = maxZ; //C'
  cube[7][0] = -maxPos; cube[7][1] = -maxPos; cube[7][2] = maxZ; //D'

  for (i = 0; i < 8; i++)
  {
    actualStatus = delta_calcInverse(cube[i][0], cube[i][1], cube[i][2], theta[0], theta[1], theta[2]);
    if (DEBUG)
    {
      Serial.print("\nStatus after calculus: ");
      Serial.println(actualStatus);
    }
    actualStatus = checkPos(actualStatus, cube[i][0], cube[i][1], cube[i][2], theta[0], theta[1], theta[2]);

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
        Serial.print(cube[i][0]); Serial.print(", ");
        Serial.print(cube[i][1]); Serial.print(", ");
        Serial.print(cube[i][2]); Serial.println("]");

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
