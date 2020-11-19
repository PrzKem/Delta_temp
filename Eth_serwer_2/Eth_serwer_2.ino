#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 1, 177 };

EthernetServer server(80); //server port

String readString;

const int greenPin = A4;
const int redPin = A5;
const int yellowPin = A6;

void setup() {
  //serial for debug
  Serial.begin(9600);
  Serial.println("\nSerial alive");

  //pin config
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);

  //ethernet setup
  Ethernet.init(53);
  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }

  if (Ethernet.linkStatus() == LinkOFF)
  {
    Serial.println("Ethernet cable is not connected.");
  }

  // start the server
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

}

void loop() {
  // Create a client connection
  EthernetClient client = server.available();
  int delimiterPos = 0;
  String diode = "";
  String value = "";
  if (client) {
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string
          readString += c;
          //Serial.print(c);
        }

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
          Serial.println(readString); //print to serial monitor for debuging

          //now output HTML data header
          if (readString.indexOf('?') >= 0) { //don't send new page
            client.println("HTTP/1.1 204");
            client.println();
            client.println();
          }
          else {
            client.println("HTTP/1.1 200 OK"); //send new page
            client.println("Content-Type: text/html");
            client.println();

            client.println("<HTML>");
            client.println("<HEAD>");
            client.println("<TITLE>Arduino GET test page</TITLE>");
            client.println("</HEAD>");
            client.println("<BODY>");

            //green LED
            client.println("<H1>Green LED</H1>");

            client.println("<a href=\"/?g&1\" target=\"greenframe\">ON</a>");
            client.println("<a href=\"/?g&0\" target=\"greenframe\">OFF</a>");

            client.println("<IFRAME name=greenframe style=\"display:none\" >");
            client.println("</IFRAME>");

            //red LED
            client.println("<H1>Red LED</H1>");

            client.println("<a href=\"/?r&1\" target=\"redframe\">ON</a>");
            client.println("<a href=\"/?r&0\" target=\"redframe\">OFF</a>");

            client.println("<IFRAME name=redframe style=\"display:none\" >");
            client.println("</IFRAME>");

            //yellow LED
            client.println("<H1>Yellow LED</H1>");

            client.println("<a href=\"/?y&1\" target=\"yellowframe\">ON</a>");
            client.println("<a href=\"/?y&0\" target=\"yellowframe\">OFF</a>");

            client.println("<IFRAME name=yellowframe style=\"display:none\" >");
            client.println("</IFRAME>");

            client.println("</BODY>");
            client.println("</HTML>");
          }

          delay(1);
          //stopping client
          client.stop();
          delimiterPos = readString.indexOf('&');
          Serial.print("Delimiter pos: ");
          Serial.println(delimiterPos);
          if (delimiterPos > 0)
          {
            diode = readString.substring(delimiterPos - 1, delimiterPos);
            value = readString.substring(delimiterPos + 1, delimiterPos + 2);
            Serial.print("diode: ");
            Serial.println(diode);
            Serial.print("value: ");
            Serial.println(value);
            switch (diode[0])
            {
              case 'r':
                digitalWrite(redPin, value[0] - 48);
                break;
              case 'g':
                digitalWrite(greenPin, value[0] - 48);
                break;
              case 'y':
                digitalWrite(yellowPin, value[0] - 48);
                break;
            }
          }
          //clearing string for next read
          readString = "";

        }
      }
    }
  }
}
