void requestRealTime()
{
  float x, y, z;
  int i;

  //Serial.println(F("Connecting..."));

  // Connect to HTTP server

  client.setTimeout(10000);
  if (!client.connect("192.168.1.100", 80)) {
    Serial.println(F("Connection failed"));
    return;
  }

  //Serial.println(F("Connected!"));

  // Send HTTP request
  client.println("GET /values.php?rN=3 HTTP/1.0");
  client.println("Host: 192.168.1.100");
  client.println("Connection: close");
  if (client.println() == 0) {
    Serial.println("Failed to send request");
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print("Unexpected response: ");
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println("Invalid response");
    return;
  }

  // Allocate the JSON document
  // Use arduinojson.org/v6/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(4) + 40;
  DynamicJsonDocument doc(capacity);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return;
  }

  // Extract values
  i = doc["canGo"].as<int>();
  x = doc["x"].as<float>();
  y = doc["y"].as<float>();
  z = doc["z"].as<float>();

  if (i == 1)
  {
    Serial.println("Response:");
    Serial.print("x: "); Serial.println(x, 2);
    Serial.print("y: "); Serial.println(y, 2);
    Serial.print("z: "); Serial.println(z, 2);
    Serial.print("Stat: "); Serial.println(i);
  }


  // Disconnect
  client.stop();

}
