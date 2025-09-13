void maintainWifi(){
    unsigned long now = millis();
  if (now - lastCheck > checkInterval) {
    lastCheck = now;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F("Wi-Fi lost. Retrying..."));

      preferences.begin("wifiCreds", true);
      String ssid = preferences.getString("ssid", "");
      String password = preferences.getString("pass", "");
      preferences.end();

      WiFi.begin(ssid.c_str(), password.c_str());

      if (!bleStarted) {
        Serial.println(F("Starting BLE for reconfiguration..."));
        startBLE();
      }
    }
  }
}


void restWifi(){
    // Reset EEPROM if button pressed
  if (digitalRead(RESET_BUTTON) == LOW) {
    Serial.println(F("\nReset button pressed! Clearing Wi-Fi credentials..."));
    preferences.begin("wifiCreds", false);
    preferences.clear();
    preferences.end();
    delay(1000);
    ESP.restart();
  }
}