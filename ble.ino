#include <WiFi.h>
#include <Preferences.h>
#include <NimBLEDevice.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <FastLED.h>
//////web socket
WebSocketsClient webSocket;

const char* ws_server = "192.168.1.108";///"192.168.1.108";  ///"192.168.30.100"
const int ws_port = 8080;
const char* espId = "LM:";
const char* pin_port = "I-R-21-19/";
String bleName;
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length);
//////
unsigned long lastCheck = 0;
const unsigned long checkInterval = 10000;
void restWifi();
void maintainWifi();
void getID();
//////
#define RESET_BUTTON   23 
#define Relay30A       21 
#define Relay10A       19 
#define IR_Transimitter      4 
#define IR_Reciver           15 
////
#define LED_PIN     5      
#define NUM_LEDS    30     
#define BRIGHTNESS  200
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
void setColorFromHex(String hexColor);
////
Preferences preferences;
bool deviceConnected = false;
bool bleStarted = false;
void connectToWiFi();
void startBLE();
///
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RESET_BUTTON, INPUT_PULLUP);  // Configure reset button
  getID();

  connectToWiFi();
  //
  //for (int i = 2; i <= 5; i++) pinMode(i, OUTPUT);
  pinMode(Relay30A, OUTPUT);
  pinMode(Relay10A, OUTPUT);
  pinMode(IR_Transimitter, OUTPUT);
///
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  setColorFromHex("#FF000000");  
  //
  webSocket.begin(ws_server, ws_port, "/");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000); 
}

void loop() {
restWifi();
maintainWifi();
 
webSocket.loop();

}
