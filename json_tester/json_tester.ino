#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>

EthernetClient client;
int i = 1;
int maxReq = 0;

void requestValues(int id = 0);
int getMetaInfo();
void requestRealTime();

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  Ethernet.init(53);
  //while (!Serial) continue;
  for(int i=0;i<20;i++)
    Serial.print("--");

  Serial.println();
  // Initialize Ethernet library
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Failed to configure Ethernet"));
    return;
  }
  delay(1000);

  maxReq = getMetaInfo();
  Serial.print("Num of pos: ");
  Serial.println(maxReq);
}

void loop() {
  if (i <= maxReq)
  {
    requestValues(i);
    //Serial.print("i: "); Serial.println(i);
    i++;
    delay(1000);
  }
  else
  {
    requestRealTime();
    delay(500);
  }
}
