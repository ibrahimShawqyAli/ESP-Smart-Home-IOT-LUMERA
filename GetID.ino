void getID() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  uint8_t m[6];
  WiFi.macAddress(m);
  // last 3 bytes -> base36 (~5 chars)
  uint32_t last3 = ((uint32_t)m[3] << 16) | ((uint32_t)m[4] << 8) | m[5];
  String shortId = String(last3, 36);
  shortId.toUpperCase();
  Serial.print("shortId: ");
   Serial.println(shortId);  // debug
  bleName = String(espId) + String(pin_port) + shortId;

  Serial.print(F("my ID is : "));
  Serial.println(bleName);
}
