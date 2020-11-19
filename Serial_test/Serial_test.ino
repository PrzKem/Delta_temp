const int yellowLed = A6;
const int greenLed = A4;
const int redLed = A5;
const int dirPin = 17;
const int pulPin = 16;
const int homeA = A0;
String odebraneDane = "";
boolean lastStatus, actualStatus;
int ilosc = 0;
int delayTime = 3;

void setup() {
  Serial.begin(9600);
  
  pinMode(yellowLed,OUTPUT);
  pinMode(greenLed,OUTPUT);
  pinMode(redLed,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(pulPin,OUTPUT);
  pinMode(homeA,INPUT_PULLUP);
  
  digitalWrite(yellowLed,LOW);
  digitalWrite(greenLed,LOW);
  digitalWrite(redLed,LOW);
  digitalWrite(dirPin,LOW);
  digitalWrite(pulPin,LOW);
  actualStatus = digitalRead(homeA);
  lastStatus = actualStatus;
}

void loop() {
  if(Serial.available()>0)
  {
    odebraneDane = Serial.readStringUntil(';');
    Serial.print("direction: ");

      if(odebraneDane[0]-'0' == 0)
      {
        digitalWrite(yellowLed,LOW);
        digitalWrite(greenLed,HIGH);
        digitalWrite(dirPin,LOW);
        Serial.println("up");
      }
      else if(odebraneDane[0]-'0' == 1)
      {
        digitalWrite(yellowLed,HIGH);
        digitalWrite(greenLed,LOW);
        digitalWrite(dirPin,HIGH);
        Serial.println("down");
      }
      else if(odebraneDane[0]-'0' == 2)
      {
        delayTime = (Serial.readStringUntil('\n')).toInt();
        Serial.print("Delay time: ");
        Serial.println(delayTime);
      }
      else
      {
        digitalWrite(yellowLed,LOW);
        digitalWrite(greenLed,LOW);
        digitalWrite(dirPin,LOW);
      }
      if(odebraneDane[0]-'0' == 1 || odebraneDane[0]-'0' == 0)
      {
        ilosc = (Serial.readStringUntil('\n')).toInt();
        Serial.print("num of steps: ");
        Serial.println(ilosc);
        for(int i=0;i<ilosc;i++)
        {
          digitalWrite(redLed,HIGH);
          digitalWrite(pulPin,HIGH);
          delay(delayTime);
          digitalWrite(redLed,LOW);
          digitalWrite(pulPin,LOW);
          delay(delayTime);
        }
      }
      
    }
  actualStatus = digitalRead(homeA);
  if(lastStatus != actualStatus)
  {
    lastStatus = actualStatus;
    if(actualStatus == LOW)
      Serial.println("A in home");
  }
}
