#define A_DIR_PIN          17
#define A_STEP_PIN         16

#define B_DIR_PIN          37
#define B_STEP_PIN         36

#define C_DIR_PIN          49
#define C_STEP_PIN         48

void setup() {
  pinMode(A_STEP_PIN,   OUTPUT);
  pinMode(A_DIR_PIN,    OUTPUT);

  pinMode(B_STEP_PIN,   OUTPUT);
  pinMode(B_DIR_PIN,    OUTPUT);

  pinMode(C_STEP_PIN,   OUTPUT);
  pinMode(C_DIR_PIN,    OUTPUT);

  digitalWrite(A_STEP_PIN, HIGH);
  digitalWrite(A_DIR_PIN, HIGH);

  digitalWrite(B_STEP_PIN, HIGH);
  digitalWrite(B_DIR_PIN, HIGH);

  digitalWrite(C_STEP_PIN, HIGH);
  digitalWrite(C_DIR_PIN, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:

}
