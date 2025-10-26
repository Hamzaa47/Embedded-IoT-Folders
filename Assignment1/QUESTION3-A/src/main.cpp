// M. Hamza
// 23-NTU-CS-1186

#include <Arduino.h>
#define ledredpin 5
#define ledyellowpin 17
#define ledgreenpin 2
#define buttonbluepin 32
#define buttongreenpin 33

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <driver/ledc.h>   // For ESP32 PWM control

// ---- OLED setup ----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---- State Variables ----
int state = 0;  // 0=OFF, 1=BLINK, 2=ON, 3=FADE
bool buttonon = false;

// ---- PWM (LEDC) Configuration ----
const int PWM_FREQ = 5000;
const int PWM_RESOLUTION = 8; // 8-bit (0-255)
const int CH_RED = 0;
const int CH_YELLOW = 1;
const int CH_GREEN = 2;

// ---- Function Prototypes ----
void bothOff();
void blink();
void bothOn();
void fadeAll();
void resetAll();
void led(const char* msg);

void setup() {
  pinMode(ledredpin, OUTPUT);
  pinMode(ledyellowpin, OUTPUT);
  pinMode(ledgreenpin, OUTPUT);

  pinMode(buttonbluepin, INPUT_PULLUP);
  pinMode(buttongreenpin, INPUT_PULLUP);

  // Setup LEDC channels
  ledcSetup(CH_RED, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(CH_YELLOW, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(CH_GREEN, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(ledredpin, CH_RED);
  ledcAttachPin(ledyellowpin, CH_YELLOW);
  ledcAttachPin(ledgreenpin, CH_GREEN);

  // Ensure LEDs are off
  ledcWrite(CH_RED, 0);
  ledcWrite(CH_YELLOW, 0);
  ledcWrite(CH_GREEN, 0);

  Wire.begin(21,22);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;);
  }
  led("READY");
}

void led(const char* msg){
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(1, 5);
  display.println(msg);
  display.display();
}

// ---- RESET (Green Button) ----
void resetAll() {
  ledcWrite(CH_RED, 0);
  ledcWrite(CH_YELLOW, 0);
  ledcWrite(CH_GREEN, 0);
  led("RESET");
  state = 0;
  delay(500);
}

// ---- ALL OFF ----
void bothOff(){
  if (digitalRead(buttonbluepin) == LOW){
    delay(300);
    state = 1;
  }
}

// ---- BLINK MODE ----
void blink(){
  while (state == 1) {
    ledcWrite(CH_RED, 255); delay(200);
    ledcWrite(CH_RED, 0);
    // Continuously check buttons as well
    if (digitalRead(buttonbluepin) == LOW) {
      delay(300);
      state = 2;
    }
    if (digitalRead(buttongreenpin) == LOW) {
      resetAll();
      return;
    }

    ledcWrite(CH_YELLOW, 255); delay(200);
    ledcWrite(CH_YELLOW, 0);

    if (digitalRead(buttonbluepin) == LOW) {
      delay(300);
      state = 2;
    }
    if (digitalRead(buttongreenpin) == LOW) {
      resetAll();
      return;
    }

    ledcWrite(CH_GREEN, 255); delay(200);
    ledcWrite(CH_GREEN, 0);

    if (digitalRead(buttonbluepin) == LOW) {
      delay(300);
      state = 2;
    }
    if (digitalRead(buttongreenpin) == LOW) {
      resetAll();
      return;
    }
  }
}

// ---- ALL ON ----
void bothOn(){
  ledcWrite(CH_RED, 255);
  ledcWrite(CH_YELLOW, 255);
  ledcWrite(CH_GREEN, 255);

  while (state == 2) {
    if (digitalRead(buttonbluepin) == LOW) {
      delay(300);
      state = 3;
    }
    if (digitalRead(buttongreenpin) == LOW) {
      resetAll();
      return;
    }
  }
}

// ---- FADE MODE ----
void fadeAll(){
  while (state == 3) {
    for (int v = 0; v <= 255; v += 20) {
      ledcWrite(CH_RED, v);
      ledcWrite(CH_YELLOW, v);
      ledcWrite(CH_GREEN, v);
      delay(20);

      if (digitalRead(buttonbluepin) == LOW) {
        delay(300);
        state = 0;
        return;
      }
      if (digitalRead(buttongreenpin) == LOW) {
        resetAll();
        return;
      }
    }
    for (int v = 255; v >= 0; v -= 20) {
      ledcWrite(CH_RED, v);
      ledcWrite(CH_YELLOW, v);
      ledcWrite(CH_GREEN, v);
      delay(20);

      if (digitalRead(buttonbluepin) == LOW) {
        delay(300);
        state = 0;
        return;
      }
      if (digitalRead(buttongreenpin) == LOW) {
        resetAll();
        return;
      }
    }
  }
}

// ---- MAIN LOOP ----
void loop() {
  if (digitalRead(buttongreenpin) == LOW) {
    resetAll();
  }

  if (state == 0){
    led("ALL OFF");
    ledcWrite(CH_RED, 0);
    ledcWrite(CH_YELLOW, 0);
    ledcWrite(CH_GREEN, 0);
    bothOff();
  }
  else if (state == 1){
    led("BLINK");
    blink();
  }
  else if (state == 2){
    led("ALL ON");
    bothOn();
  }
  else if (state == 3){
    led("FADE");
    fadeAll();
  }
}
