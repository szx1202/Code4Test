/*
  Model Railroad Speedometer
  -------------------------------------------
  Modified by:
  Jasen Krueger on 09/25/2021
  Changed inputs to pins 2 and 3
  Changed to 16x2 LCD Display
  -------------------------------------------
  Originaly by:
  Bobot's Trains and Maker Zone
  https://www.youtube.com/user/BobotsTrains

  03/11/2017
  Complete code re-write adding detection
  in both directions and continuous train
  length detection of before reset
  -------------------------------------------
  Original concept by:
  Steve Spence
  https://www.youtube.com/user/sspence65
  -------------------------------------------
  06/29/2024
  Modified by:
  SetZinni
  Simplified and Adapted to My Railway Project 
  http://szweb.eu 
  -------------------------------------------
*/

#define DEBUG //comment out to stop debug

//I2C Setup
#include <Wire.h> // Library for I2C communication
// Wiring: SDA pin is connected to A4 and SCL pin to A5 Uno, Nano
// Wiring: SDA pin is connected to 20 and SCL pin to 21 Mega

//LCD Setup
#include <LiquidCrystal_I2C.h> // Library for LCD
//Connect to LCD via I2C, default address 0x27 (A0-A2 not jumpered)
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2); // Change to (0x27,16,2) for 16x2 LCD.

//user variables
unsigned int distancemm = 8170.2; //distance between sensors (mm)
float distance;   //distance between sensors (inches)
int scale = 87;   //scale of model to report scale speed  (O-Scale = 48, HO-Scale - 87)

int sensor1 = 2;   //sensor1 digital pin
int sensor2 = 3;   //sensor2 digital pin
int s1Open = 1;   //is START sensor open/uncovered? (goes to 0 if covered/triggered)
int s2Open = 1;   //is END sensor open/uncovered? (goes to 0 if covered/triggered)

float startmillis, finishmillis, elapsed, miles, hours, mph, scaleMPH;
bool started1 = false;    // set to TRUE if START sensor has been triggered
bool started2 = false;    // set to TRUE if END sensor has been triggered

bool initialized = true;   //set to TRUE once all sensors are cleared for a period of time and ready to read
float previousmillis=0;
byte direction;

void setup() {
  distance = distancemm / 25.4;
  Serial.begin(9600);
  pinMode(sensor1, INPUT_PULLUP);
  pinMode(sensor2, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.clear();   //clear display
}


void loop() {

  //read respective sensor if not already triggered
  if (!started1) {  
  s1Open = digitalRead(sensor1);
  
  #ifdef DEBUG
    Serial.print("Started1= ");
    Serial.println(started1);
    Serial.print("s1Open=   ");
    Serial.println(s1Open);
    Serial.println("======================");
  #endif
  }
  
  if (!started2) {
    s2Open = digitalRead(sensor2);
    #ifdef DEBUG
      Serial.print("Started2= ");
      Serial.println(started2);
      Serial.print("s2Open=   ");
      Serial.println(s2Open);
      Serial.println("======================");
    #endif
  }

   bool goodtogo;
   if ((!started1 && !started2) && (s1Open && s2Open)){
      goodtogo = true;
      #ifdef DEBUG
        Serial.println("goodtogo");
      #endif
    }
    else {
        goodtogo = false;
        #ifdef DEBUG
          Serial.println("NO goodtogo");
        #endif
    }
   
//    // neither sensor was previously triggered, nor triggered in last several seconds, so "reset"/initialize vars and display
//    if (goodtogo) {
//      initialized = true;
//      started1 = false;
//      started2 = false;
//      previousmillis = millis();
//      lcd.setCursor(0, 0);
//      lcd.setCursor(0, 1);
//     Serial.println("Ready to measure train speed!    ");
//    }

 // Sensor 1 is triggered, neither sensor was previously triggered, and we were in initialize mode
   
    if ((!s1Open) && (!started1 && !started2) && (initialized)) {
    started1 = true;
    initialized = false;
    startmillis = millis();
    direction = 1;
    #ifdef DEBUG
      Serial.print("Direction 1  ");
      Serial.println (direction);
    #endif
   }
//
//  Sensor 2 is triggered, neither sensor was previously triggered, and we were in initialize mode
  if ((!s2Open) && (!started1 && !started2) && (initialized)) {
    started2 = true;
    initialized = false;
    startmillis = millis();
    direction = 0;
    #ifdef DEBUG
      Serial.print("Direction 2  ");
      Serial.println(direction);
    #endif
  }

  // Sensor 1 is triggered, neither sensor was previously triggered, and we were in initialize mode
  if ((!s1Open) && (!started1 && !started2) && (initialized)) {
    started1 = true;
    initialized = false;
    startmillis = millis(); 
    direction = 1;
  }

  // Sensor 2 is triggered, neither sensor was previously triggered, and we were in initialize mode
  if ((!s2Open) && (!started1 && !started2) && (initialized)) {
    started2 = true;
    initialized = false;
    startmillis = millis();
    direction = 0;
  }

  //a first sensor (START) was previously triggered and now a second sensor (END) is triggered, calculate and display scale MPH
  if (((started1) && (!s2Open)) || ((started2) && (!s1Open))) {
    finishmillis = millis();
    elapsed = finishmillis - startmillis;
    elapsed = elapsed / 1000;    //seconds
    miles = distance / 63360;   //miles
    hours = elapsed / 3600;   //hours
    mph = miles / hours;
    scaleMPH = mph * scale;
    #ifdef DEBUG
      Serial.print("Seconds: ");
      Serial.println(elapsed);
    #endif
    
    // print scale MPH on display
    lcd.clear();   //clear display
    if (scaleMPH < 100) {
      lcd.setCursor(1, 0);
    }
    else {
      lcd.setCursor(1, 0);
    }

    lcd.print(scaleMPH);
    lcd.print(" MPH");
    lcd.setCursor(11, 0);
    
    if (direction == 1) {
      lcd.print("East ");  //or North
    }
    else
    {
      lcd.print("West "); //or South
    }
    if (scaleMPH * 1.609344 < 100) {
      lcd.setCursor(1, 1);
    }
    else {
      lcd.setCursor(0, 1);
    }
    lcd.print(scaleMPH * 1.609344);
    
    lcd.print(" KPH");
    lcd.setCursor(11, 1);
    lcd.print("Bound");
    #ifdef DEBUG
      Serial.println(" KPH");
      Serial.print(scaleMPH * 1.609344);
    #endif
    
    // reset
    delay(5000);   //wait
    lcd.setCursor(0, 0);
    lcd.print("Waiting for     ");
    lcd.setCursor(0, 1);
    lcd.print("train to pass.  ");
    started1 = false;
    started2 = false;
    initialized = true;
  }
}
