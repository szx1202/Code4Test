/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
  https://randomnerdtutorials.com/esp32-ssd1306-oled-display-arduino-ide/
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMonoOblique24pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Default Pin SDA 21 and SCL 22
//#define I2C_SDA 32
//#define I2C_SCL 23

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setFont(&FreeMonoOblique24pt7b);
  display.setFont();
  
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("T= 26.5");
  display.setCursor(0, 30);
  display.println("H= 1013.3");
  display.setCursor(0, 50);
  display.println("E= 130");
  display.display(); 
  delay(3000);
}

void loop() {

  
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("T= 26.5");
  display.setCursor(0, 30);
  display.println("H= 1013.3");
  display.setCursor(0, 50);
  display.println("E= 130");
  display.display(); 
  
  delay(3000);
  //display.ssd1306_command(SSD1306_DISPLAYOFF);
  //delay(5000);
  display.ssd1306_command(SSD1306_DISPLAYON);

}
