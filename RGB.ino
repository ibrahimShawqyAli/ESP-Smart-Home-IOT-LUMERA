void setColorFromHex(String hexColor) {
  if (hexColor.length() != 9 || hexColor[0] != '#') return;

  String rStr = hexColor.substring(3, 5);
  String gStr = hexColor.substring(5, 7);
  String bStr = hexColor.substring(7, 9);

  uint8_t r = strtoul(rStr.c_str(), NULL, 16);
  uint8_t g = strtoul(gStr.c_str(), NULL, 16);
  uint8_t b = strtoul(bStr.c_str(), NULL, 16);

  // Fill all LEDs with this color
  fill_solid(leds, NUM_LEDS, CRGB(g, r, b));
  FastLED.show();
}