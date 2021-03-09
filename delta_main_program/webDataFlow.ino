const char* serverIP = "192.168.8.10";

void ethernetSetup()
{
  Ethernet.init(53);
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  if (!Ethernet.begin(mac)) {
    if (DEBUG)
      Serial.println(F("Failed to configure Ethernet"));
    return;
  }
}

int getMetaInfo()
{
  int respo = 0;
  if (DEBUG)
  {
    //Serial.println(F("Connecting..."));
  }

  // Connect to HTTP server

  client.setTimeout(10000);
  if (!client.connect(serverIP, 80)) {
    if (DEBUG)
    {
      Serial.println(F("Connection failed"));
    }
    return;
  }
  //if (DEBUG)
      //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println("GET /values.php?rN=1 HTTP/1.0");
  client.print("Host: ");
  client.println(serverIP);
  client.println("Connection: close");
  if (client.println() == 0) {
    if (DEBUG)
    {
      Serial.println("Failed to send request");
    }
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    if (DEBUG)
    {
      Serial.print("Unexpected response: ");
      Serial.println(status);
    }
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    if (DEBUG)
      Serial.println("Invalid response");
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(1) + 40;
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    if (DEBUG)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
    }
    return;
  }

  respo = doc["numOfValues"].as<int>();
  return respo;
}

void requestValues(int id, float* x, float* y, float* z, int* i, int* q1, int* q2)
{
  String getQuestion = "GET /values.php?rN=2&id=";
  getQuestion += id;
  getQuestion += " HTTP/1.0";

  //Serial.println(F("Connecting..."));

  // Connect to HTTP server

  client.setTimeout(10000);
  if (!client.connect(serverIP, 80)) {
    if (DEBUG)
      Serial.println(F("Connection failed"));
    return;
  }

  //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println(getQuestion);
  client.print("Host: ");
  client.println(serverIP);
  client.println("Connection: close");
  if (client.println() == 0) {
    if (DEBUG)
      Serial.println("Failed to send request");
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    if (DEBUG)
    {
      Serial.print("Unexpected response: ");
      Serial.println(status);
    }
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    if (DEBUG)
      Serial.println("Invalid response");
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(6) + 40;
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    if (DEBUG)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
    }
    return;
  }

  // Extract values
  *i = doc["status"].as<int>();
  *x = doc["x"].as<float>();
  *y = doc["y"].as<float>();
  *z = doc["z"].as<float>();
  *q1 = doc["q1"].as<int>();
  *q2 = doc["q2"].as<int>(); 

  // Disconnect
  client.stop();
}

void requestRealTime(int *i, float* x, float* y, float* z, int* q1, int* q2)
{
  // Connect to HTTP server

  client.setTimeout(10000);
  if (!client.connect(serverIP, 80)) {
    if (DEBUG)
    {
      Serial.println(F("Connection failed"));
    }
    return;
  }

  // Send HTTP request
  client.println("GET /values.php?rN=3 HTTP/1.0");
  client.print("Host: ");
  client.println(serverIP);
  client.println("Connection: close");
  if (client.println() == 0) {
    if (DEBUG)
    {
      Serial.println("Failed to send request");
    }
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    if (DEBUG)
    {
      Serial.print("Unexpected response: ");
      Serial.println(status);
    }
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    if (DEBUG)
    {
      Serial.println("Invalid response");
    }
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(6) + 40;
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    if (DEBUG)
    {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.f_str());
    }
    return;
  }

  // Extract values
  *i = doc["canGo"].as<int>();
  *x = doc["x"].as<float>();
  *y = doc["y"].as<float>();
  *z = doc["z"].as<float>();
  *q1 = doc["q1"].as<int>();
  *q2 = doc["q2"].as<int>(); 

  if (*i == 1)
  {
    if (DEBUG)
    {
      Serial.println("Response:");
      Serial.print("x: "); Serial.println(*x, 2);
      Serial.print("y: "); Serial.println(*y, 2);
      Serial.print("z: "); Serial.println(*z, 2);
      Serial.print("q1: "); Serial.println(*q1);
      Serial.print("q2: "); Serial.println(*q2);
      Serial.print("Stat: "); Serial.println(*i);
    }
  }


  // Disconnect
  client.stop();

}
