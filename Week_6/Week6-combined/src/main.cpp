#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT11
#define LDR_PIN 36
#define SDA_PIN 21
#define SCL_PIN 22
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 failed");
    for (;;);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 error");
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Temperature & Humidity");
  display.setCursor(0, 16);
  display.print("Temp: ");
  display.setTextSize(2);
  display.print(temperature);
  display.setTextSize(1);
  display.println(" C");
  display.setCursor(0, 32);
  display.print("Humidity: ");
  display.setTextSize(2);
  display.print(humidity);
  display.setTextSize(1);
  display.println(" %");
  display.display();

  Serial.printf("Temp: %.2f C | Humidity: %.2f %%\n", temperature, humidity);
  delay(3000);

  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Light Intensity");
  display.setCursor(0, 16);
  display.print("ADC: ");
  display.println(adcValue);
  display.setCursor(0, 32);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.println(" V");
  display.display();

  Serial.printf("ADC: %d | Voltage: %.2f V\n", adcValue, voltage);
  delay(3000);
}
