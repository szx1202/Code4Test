// *****************************************************sketch di sviluppo *************************

// Libraries
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
// OLED display TWI address
#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);

// ****************************************** Main Setup Routine **********************
void setup() {
Serial.begin(9600);


//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif

// initialize and clear display
display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
display.clearDisplay();
display.display();
}
// ************************************************************************************

// ****************************************** Main Loop Routine ***********************
void loop() {

int row, col;
//long value;
//String msg;

//ldr_buzzer (); // Attiva LDR Routine

//value=dht22('T');
//msg = "Temperature ";
row=1;
col=1;
oled("Temperature ",dht22('T'),row,col);

//value=dht22('H');
//msg = "Humidity ";

row=row+20;
oled("humidity ",dht22('H'),row,col);
// fade_led();


delay(1000);
}
//*****************************************************************************************
//*****************************************************************************************


//**************************** OLED Display 128X64 Routine ********************************
void oled(String str, float value, int r, int c) {



  // display a pixel in each corner of the screen
  display.drawPixel(0, 0, WHITE);
  display.drawPixel(127, 0, WHITE);
  display.drawPixel(0, 63, WHITE);
  display.drawPixel(127, 63, WHITE);

  // display a line of text
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(c,r);
  display.print(str);
  display.print(value);

  // update display with all of the above graphics
  display.display();
}
//***************************************************************************************


// ****************************************** DHT22 Routine **********************
float dht22(char TorH)
{
  //Constants
  #define DHTPIN 8     // what pin we're connected to
  #define DHTTYPE DHT22   // DHT 22  (AM2302)
  DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor for normal 16mhz Arduino
  
  //Variables
  float hum;  //Stores humidity value
  float temp;  //Stores temperature value

  dht.begin();

  //delay(dht.getMinimumSamplingPeriod());
  delay(1000);
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();
  //Print temp and humidity values to serial monitor
  if (TorH=='H'){
    Serial.print(F("Humidity: "));
    Serial.print(hum);
    return hum;}
  if (TorH=='T'){
    Serial.print(F(" %, Temp: "));
    Serial.print(temp);
    Serial.println(F(" Celsius"));
    return temp;}
}
