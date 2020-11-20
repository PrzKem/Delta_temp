#define PULS_PER_REVOLUTION 800
#define GEAR_RATIO 43
#define NUM_STEPPERS 3

#define A_PUL_HIGH             PORTH |=  0b00000010;
#define A_PUL_LOW              PORTH &= ~0b00000010;

#define B_PUL_HIGH             PORTC |=  0b00000010;
#define B_PUL_LOW              PORTC &= ~0b00000010;

#define C_PUL_HIGH             PORTL |=  0b00000010;
#define C_PUL_LOW              PORTL &= ~0b00000010;

#define TIMER1_INTERRUPTS_ON    TIMSK1 |=  (1 << OCIE1A);
#define TIMER1_INTERRUPTS_OFF   TIMSK1 &= ~(1 << OCIE1A);



volatile byte remainingSteppersFlag = 0;
volatile byte nextStepperFlag = 0;
volatile int ind = 0;
volatile unsigned int intervals[100];
volatile stepperInfo steppers[NUM_STEPPERS];

void aStep() {
  A_PUL_HIGH
  A_PUL_LOW
}
void aDir(int dir) {
  digitalWrite(A_DIR_PIN, dir);
}

void bStep() {
  B_PUL_HIGH
  B_PUL_LOW
}
void bDir(int dir) {
  digitalWrite(B_DIR_PIN, dir);
}

void cStep() {
  C_PUL_HIGH
  C_PUL_LOW
}
void cDir(int dir) {
  digitalWrite(C_DIR_PIN, dir);
}

void stepperSetup()
{
  Serial.begin(9600);
  pinMode(A_PUL_PIN,   OUTPUT);
  pinMode(A_DIR_PIN,    OUTPUT);

  pinMode(B_PUL_PIN,   OUTPUT);
  pinMode(B_DIR_PIN,    OUTPUT);

  pinMode(C_PUL_PIN,   OUTPUT);
  pinMode(C_DIR_PIN,    OUTPUT);

  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 1000;                             // compare value
  TCCR1B |= (1 << WGM12);                   // CTC mode
  TCCR1B |= ((1 << CS11) | (1 << CS10));    // 64 prescaler
  interrupts();

  steppers[0].dirFunc = aDir;
  steppers[0].stepFunc = aStep;
  steppers[0].acceleration = 1000;
  steppers[0].minStepInterval = 50;

  steppers[1].dirFunc = bDir;
  steppers[1].stepFunc = bStep;
  steppers[1].acceleration = 1000;
  steppers[1].minStepInterval = 50;

  steppers[2].dirFunc = cDir;
  steppers[2].stepFunc = cStep;
  steppers[2].acceleration = 1000;
  steppers[2].minStepInterval = 50;
}

void resetStepper(volatile stepperInfo& si) {
  si.c0 = si.acceleration;
  si.d = si.c0;
  si.di = si.d;
  si.stepCount = 0;
  si.n = 0;
  si.rampUpStepCount = 0;
  si.movementDone = false;
  si.speedScale = 1;

  float a = si.minStepInterval / (float)si.c0;
  a *= 0.676;

  float m = ((a*a - 1) / (-2 * a));
  float n = m * m;

  si.estStepsToSpeed = n;
}

float getDurationOfAcceleration(volatile stepperInfo& s, unsigned int numSteps) {
  float d = s.c0;
  float totalDuration = 0;
  for (unsigned int n = 1; n < numSteps; n++) {
    d = d - (2 * d) / (4 * n + 1);
    totalDuration += d;
  }
  return totalDuration;
}

void prepareMovement(int whichMotor, long steps) {
  volatile stepperInfo& si = steppers[whichMotor];
  si.dirFunc( steps < 0 ? HIGH : LOW );
  si.dir = steps > 0 ? 1 : -1;
  si.totalSteps = abs(steps);
  resetStepper(si);
  
  remainingSteppersFlag |= (1 << whichMotor);

  unsigned long stepsAbs = abs(steps);

  if ( (2 * si.estStepsToSpeed) < stepsAbs ) {
    // there will be a period of time at full speed
    unsigned long stepsAtFullSpeed = stepsAbs - 2 * si.estStepsToSpeed;
    float accelDecelTime = getDurationOfAcceleration(si, si.estStepsToSpeed);
    si.estTimeForMove = 2 * accelDecelTime + stepsAtFullSpeed * si.minStepInterval;
  }
  else {
    // will not reach full speed before needing to slow down again
    float accelDecelTime = getDurationOfAcceleration( si, stepsAbs / 2 );
    si.estTimeForMove = 2 * accelDecelTime;
  }
}

void setNextInterruptInterval() {

  bool movementComplete = true;

  unsigned long mind = 999999;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di < mind ) {
      mind = steppers[i].di;
    }
  }

  nextStepperFlag = 0;
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! steppers[i].movementDone )
      movementComplete = false;
    if ( ((1 << i) & remainingSteppersFlag) && steppers[i].di == mind )
      nextStepperFlag |= (1 << i);
  }

  if ( remainingSteppersFlag == 0 ) {
    TIMER1_INTERRUPTS_OFF
    OCR1A = 65500;
  }

  OCR1A = mind;
}

ISR(TIMER1_COMPA_vect)
{
  unsigned int tmpCtr = OCR1A;

  OCR1A = 65500;

  for (int i = 0; i < NUM_STEPPERS; i++) {

    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;

    if ( ! (nextStepperFlag & (1 << i)) ) {
      steppers[i].di -= tmpCtr;
      continue;
    }

    volatile stepperInfo& s = steppers[i];

    if ( s.stepCount < s.totalSteps ) {
      s.stepFunc();
      s.stepCount++;
      s.stepPosition += s.dir;
      if ( s.stepCount >= s.totalSteps ) {
        s.movementDone = true;
        remainingSteppersFlag &= ~(1 << i);
      }
    }

    if ( s.rampUpStepCount == 0 ) {
      s.n++;
      s.d = s.d - (2 * s.d) / (4 * s.n + 1);
      if ( s.d <= s.minStepInterval ) {
        s.d = s.minStepInterval;
        s.rampUpStepCount = s.stepCount;
      }
      if ( s.stepCount >= s.totalSteps / 2 ) {
        s.rampUpStepCount = s.stepCount;
      }
      s.rampUpStepTime += s.d;
    }
    else if ( s.stepCount >= s.totalSteps - s.rampUpStepCount ) {
      s.d = (s.d * (4 * s.n + 1)) / (4 * s.n + 1 - 2);
      s.n--;
    }

    s.di = s.d * s.speedScale; // integer
  }

  setNextInterruptInterval();

  TCNT1  = 0;
}

void runAndWait() {
  adjustSpeedScales();
  setNextInterruptInterval();
  TIMER1_INTERRUPTS_ON
  while ( remainingSteppersFlag );
  remainingSteppersFlag = 0;
  nextStepperFlag = 0;
}

void adjustSpeedScales() {
  float maxTime = 0;
  
  for (int i = 0; i < NUM_STEPPERS; i++) {
    if ( ! ((1 << i) & remainingSteppersFlag) )
      continue;
    if ( steppers[i].estTimeForMove > maxTime )
      maxTime = steppers[i].estTimeForMove;
  }

  if ( maxTime != 0 ) {
    for (int i = 0; i < NUM_STEPPERS; i++) {
      if ( ! ( (1 << i) & remainingSteppersFlag) )
        continue;
      steppers[i].speedScale = maxTime / steppers[i].estTimeForMove;
    }
  }
}

void goToPos(float theta1, float theta2, float theta3)
{
  if ( theta1 == 0.0)
    theta1 = 1;
  if ( theta2 == 0.0)
    theta2 = 1;
  if ( theta3 == 0.0)
    theta3 = 1;
  prepareMovement(0, (int)(-theta1 * PULS_PER_REVOLUTION * GEAR_RATIO / 360.0));
  prepareMovement(1, (int)(-theta2 * PULS_PER_REVOLUTION * GEAR_RATIO / 360.0));
  prepareMovement(2, (int)(-theta3 * PULS_PER_REVOLUTION * GEAR_RATIO / 360.0));
  runAndWait();
}
