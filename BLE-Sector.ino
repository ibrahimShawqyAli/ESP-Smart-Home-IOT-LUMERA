#define SERVICE_UUID        "20ca01a6-0680-4080-8504-146f887aec8b"
#define CHARACTERISTIC_UUID "05fa5a32-5e0c-4c79-850e-c11c497134e7"

bool myConnectFlag = false;
class MyServerCallbacks : public NimBLEServerCallbacks {
  void onConnect(NimBLEServer *pServer) {
    deviceConnected = true;
  }

  void onDisconnect(NimBLEServer *pServer) {
    deviceConnected = false;
    Serial.println("Device disconnected");
    NimBLEDevice::getAdvertising()->start();
    Serial.println("BLE advertising restarted");
 
    delay(1500);
   ESP.restart();

  }
};

class MyCallbacks : public NimBLECharacteristicCallbacks {
  void onWrite(NimBLECharacteristic *pCharacteristic) {
    // String value = pCharacteristic->getValue();
std::string raw = pCharacteristic->getValue();
String value = String(raw.c_str());

    if (value.length() > 0) {
      Serial.println("Received BLE string: " + value);

      int wifiIndex = value.indexOf("wifi:");
      int passIndex = value.indexOf("|pass:");

      if (wifiIndex != -1 && passIndex != -1) {
        String ssid = value.substring(wifiIndex + 5, passIndex);
        String password = value.substring(passIndex + 6);

        Serial.println("Parsed SSID: " + ssid);
        Serial.println("Parsed PASS: " + password);

        preferences.begin("wifiCreds", false);
        preferences.putString("ssid", ssid);
        preferences.putString("pass", password);
        preferences.end();

        Serial.println(F("Credentials saved. Waiting for disconnect..."));
        myConnectFlag = true;
      } else {
        Serial.println(F("Invalid BLE format. Use wifi:SSID|pass:PASSWORD"));
      }
    }
  }
};

void startBLE() {
  if (bleStarted) return;
  
  NimBLEDevice::init(bleName.c_str());
  NimBLEServer *pServer = NimBLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  NimBLEService *pService = pServer->createService(SERVICE_UUID);

NimBLECharacteristic *pCharacteristic = pService->createCharacteristic(
  CHARACTERISTIC_UUID,
  NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE
);


  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("Send WiFi credentials");
  pService->start();

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->start();

  Serial.println("BLE started and advertising");
  bleStarted = true;
}


void connectToWiFi() {
  preferences.begin("wifiCreds", true);
  String ssid = preferences.getString("ssid", "");
  String password = preferences.getString("pass", "");
  preferences.end();

  if (ssid == "" || password == "") {
    Serial.println(F("No Wi-Fi credentials found. Starting BLE..."));
    for(;;){
      startBLE();
    }
    return;
  }

  Serial.println("Connecting to Wi-Fi: " + ssid);
  WiFi.begin(ssid.c_str(), password.c_str());

  unsigned long startAttemptTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 15000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(F("\nWi-Fi Connected!"));
    Serial.println("IP Address: " + WiFi.localIP().toString());
  } else {
    Serial.println(F("\nFailed to connect. Starting BLE..."));
     for(;;){
      startBLE();
    }
  }
}
