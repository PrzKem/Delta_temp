#define DIR 2 //NWM które Piny przypisać
#define STEP 3 //zmiana to nie problem
#define ENA 4
#define STEPS 400 //nie jestem pewien ile skoków to pełen obrót silnika
//nie jestem pewien jak bardzo to chcemy przyspieszać

void przyspieszanie()
{
 int start_speed=2000; //okreslenie predkosci
 int final_speed=300;
 int delays[STEPS] ;
 float angle =2*PI/STEPS; //wartosci do wzoru
 float accel =0.01;
 float c0= 2000*sqrt(2*angle/accel)*0.676;;
 float last_delay;
 for(int i =0;i<STEPS;i++){
  float d=c0;
  if(i>0)
  d=last_delay-(2*last_delay)/(4*i+1); //wzory aby przyspieszanie było ciągle
  if (d< final_speed)
  d=final_speed; //zabezpieczenie zeby sie nie rozpedzało w nieskonczonosc
  delays[i]=d;
  last_delay=d;
  }
  //przyspieszanie
  for(int i = 0;i< STEPS;i++){
  digitalWrite(STEP,HIGH);
  delayMicroseconds(delays[i]);
    digitalWrite(STEP,LOW);

  }
  //zwalnianie
    for(int i = 0;i< STEPS;i++){
  digitalWrite(STEP,HIGH);
  delayMicroseconds(delays[STEPS-i]);
    digitalWrite(STEP,LOW);

  }
  }
 

void setup(){
  pinMode(DIR, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(ENA ,OUTPUT);

}
void loop(){
  digitalWrite(ENA,LOW);
  digitalWrite(DIR,LOW);
  przyspieszanie();
    digitalWrite(DIR,HIGH);
  przyspieszanie();
  
}
