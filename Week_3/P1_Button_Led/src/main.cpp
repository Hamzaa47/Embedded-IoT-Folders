
#include <Arduino.h>


// #define LED_PIN 2

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2   // fallback; adjust if your board uses another pin
#endif

// Set this to true if your LED is active-low (many ESP32-C3 boards)
const bool ACTIVE_LOW = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Start with LED off (respect active level)
  digitalWrite(LED_BUILTIN, ACTIVE_LOW ? HIGH : LOW);
}

void loop() {
  // toggle
  static bool on = false;
  on = !on;
  digitalWrite(LED_BUILTIN, (on ^ ACTIVE_LOW) ? HIGH : LOW);
  delay(500); // 0.5s on, 0.5s off
}
