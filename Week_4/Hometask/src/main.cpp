/*23-NTU-CS-1186*/


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire,
-1);
void setup() {
 Wire.begin();
 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.clearDisplay();
 display.setTextSize(1);
 display.setTextColor(SSD1306_WHITE);
 display.setCursor(10, 10);
 display.println("M. Hamza");
 display.setCursor(10, 20);
 display.println("23-NTU-CS-1186");
 display.drawRect(5,5,122,58,SSD1306_WHITE);
 display.display();
}
void loop(){

}