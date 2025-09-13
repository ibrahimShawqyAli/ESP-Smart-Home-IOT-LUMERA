

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WebSocket] Disconnected");
      break;

    case WStype_CONNECTED:
      Serial.println("[WebSocket] Connected");
        {
    String regMsg = String("{\"type\":\"register\",\"espId\":\"") + bleName + "\"}";
    webSocket.sendTXT(regMsg);
    Serial.println(F("[WebSocket] Sent registration"));
  }
      break;

    case WStype_TEXT:
      Serial.println(F("[WebSocket] Received text:"));
      Serial.println((char*)payload);

      // Parse JSON
      {
        DynamicJsonDocument doc(256);
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print("[JSON] Deserialization failed: ");
          Serial.println(error.c_str());
          return;
        }

        int pin = doc["pin"];
        String state = doc["state"];
  if(state.startsWith("#"))
  {
  setColorFromHex(state);
  Serial.print(F("Set Color to RGB"));
  }
  else{
          Serial.print(F("[ACTION] Set pin "));
        Serial.print(pin);
        Serial.print(" to ");
        Serial.println(state);
        digitalWrite(pin, (state == "on") ? HIGH : LOW);
  }
  
      }
      break;
  }
}
