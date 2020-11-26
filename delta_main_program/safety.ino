const int safetyPin = 2;

void safetyISR()
{
  canMove = false;
}

void safetySetup()
{
  pinMode(safetyPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(safetyPin), safetyISR, FALLING);
}
