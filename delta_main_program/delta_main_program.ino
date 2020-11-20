#define A_DIR_PIN         17
#define A_PUL_PIN         16
#define A_HOME            A0

#define B_DIR_PIN         37
#define B_PUL_PIN         36
#define B_HOME            A1

#define C_DIR_PIN         49
#define C_PUL_PIN         48
#define C_HOME            A2

#define MANUAL_SWITCH     A3

#define GREEN_LED         A4
#define RED_LED           A5
#define YELLOW_LED        A6

#define E_VALUE           50.0
#define F_VALUE           150.0
#define RF_VALUE          100.0
#define RE_VALUE          320.0

#define MIN_X             -83.846
#define MAX_X             83.846

#define MIN_Y             -83.846
#define MAX_Y             83.846

#define MIN_Z             -392.894
#define MAX_Z             -225.203

#define MIN_THETA1        -35.22 //polozenie czujnika krancowego jest minimum
#define MAX_THETA1        104.04

#define MIN_THETA2        -35.22 //polozenie czujnika krancowego jest minimum
#define MAX_THETA2        106.32

#define MIN_THETA3        -35.22 //polozenie czujnika krancowego jest minimum
#define MAX_THETA3        106.32

struct stepperInfo {
  // externally defined parameters
  float acceleration;
  volatile unsigned long minStepInterval; // ie. max speed, smaller is faster
  void (*dirFunc)(int);
  void (*stepFunc)();

  // derived parameters
  unsigned int c0;                // step interval for first step, determines acceleration
  long stepPosition;              // current position of stepper (total of all movements taken so far)

  // per movement variables (only changed once per movement)
  volatile int dir;                        // current direction of movement, used to keep track of position
  volatile unsigned int totalSteps;        // number of steps requested for current movement
  volatile bool movementDone = false;      // true if the current movement has been completed (used by main program to wait for completion)
  volatile unsigned int rampUpStepCount;   // number of steps taken to reach either max speed, or half-way to the goal (will be zero until this number is known)
  volatile unsigned long estStepsToSpeed;  // estimated steps required to reach max speed
  volatile unsigned long estTimeForMove;   // estimated time (interrupt ticks) required to complete movement
  volatile unsigned long rampUpStepTime;
  volatile float speedScale;               // used to slow down this motor to make coordinated movement with other motors

  // per iteration variables (potentially changed every interrupt)
  volatile unsigned int n;                 // index in acceleration curve, used to calculate next interval
  volatile float d;                        // current interval length
  volatile unsigned long di;               // above variable truncated
  volatile unsigned int stepCount;         // number of steps completed in current movement
};

void setup() {
  pinSetup();
  stepperSetup();

  moveHome(A_PUL_PIN, A_DIR_PIN, A_HOME);
  blynkLED(RED_LED, 200, 3);
  moveHome(B_PUL_PIN, B_DIR_PIN, B_HOME);
  blynkLED(RED_LED, 200, 3);
  goToPos(35.22,35.22,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
