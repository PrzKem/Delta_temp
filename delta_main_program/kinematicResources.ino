

int checkPos(int status, float theta1, float theta2, float theta3, float x0, float y0, float z0)
{
  //refer to: https://www.marginallyclever.com/other/samples/fk-ik-test.html
  const float MIN_X = -83.846;
  const float MAX_X = 83.846;

  const float MIN_Y = -83.846;
  const float MAX_Y = 83.846;

  const float MIN_Z = -392.894;
  const float MAX_Z = -225.203;

  const float MIN_THETA1 = -35.22; //polozenie czujnika krancowego jest minimum
  const float MAX_THETA1 = 104.04;

  const float MIN_THETA2 = -35.22; //polozenie czujnika krancowego jest minimum
  const float MAX_THETA2 = 106.32;

  const float MIN_THETA3 = -35.22; //polozenie czujnika krancowego jest minimum
  const float MAX_THETA3 = 106.32;

  if (DEBUG && status == 0)
  {
    Serial.print("\ntheta1<MAX_THETA1\t");
    Serial.println(theta1 < MAX_THETA1);
    Serial.print("theta1>MIN_THETA1\t");
    Serial.println(theta1 < MIN_THETA1);

    Serial.print("theta2<MAX_THETA2\t");
    Serial.println(theta2 < MAX_THETA2);
    Serial.print("theta2>MIN_THETA2\t");
    Serial.println(-theta2 > MIN_THETA2);

    Serial.print("theta3<MAX_THETA3\t");
    Serial.println(theta3 < MAX_THETA3);
    Serial.print("theta3>MIN_THETA3\t");
    Serial.println(-theta3 > MIN_THETA3);

    Serial.print("\nx0 > MIN_X\t");
    Serial.println(x0 > MIN_X);
    Serial.print("x0 < MAX_X\t");
    Serial.println(x0 < MAX_X);
    
    Serial.print("y0 > MIN_Y\t");
    Serial.println(y0 > MIN_Y);
    Serial.print("y0 < MAX_Y\t");
    Serial.println(y0 < MAX_Y);

    Serial.print("z0 > MIN_Z\t");
    Serial.println(z0 > MIN_Z);
    Serial.print("z0 < MAX_Z\t");
    Serial.println(-z0 > MAX_Z);
  }

  if (!(theta1 < MAX_THETA1 && theta1 > MIN_THETA1 && theta2 < MAX_THETA2 && -theta2 > MIN_THETA2 && theta3 < MAX_THETA3 && -theta3 > MIN_THETA3) && status == 0)
    status = -2;
  if (!(x0 > MIN_X && x0 < MAX_X && y0 > MIN_Y && y0 < MAX_Y && z0 > MIN_Z && -z0 > MAX_Z) && status == 0)
    status = -3;

  return status;
}
