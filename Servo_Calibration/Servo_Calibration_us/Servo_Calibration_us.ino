#include <Wire.h>
#include <Servo.h>

Servo Servo1; //Il nome del servo è Servo1

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 9; // address of servo to be calibrated

//// These are the rounded 'minimum' microsecond length based on the minimum pulse of xxx
//With FREQ set to 50Hz that's a 20,000 us cycle length. The 12 bit PWMDriver calculates things in 1/4096ths of that length. So 20,000 * 1491/4096 = around 7000

#define USMIN  350 // 500 for MG90S 500 for SG90
#define USMAX  2600 // per 2400 MG90S e 2400 per 5SG90
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

String readString;
int pos;

void setup() {
  Serial.begin(9600);
  pos=1475; // Initial position
  Servo1.attach (servonum);

  Serial.println("Servo calibration");
  Serial.println("Use this to calibrate your servo to find the range of movement required");
  Serial.println("The servo should start close to the centre of the range");
  Serial.println("Type a value followed by a + to move in one direction or a valu followed by a - to move in the other direction");
  Serial.println("For example 100+ to 200-");  
  Serial.println("To move to a specific location use strings like 900x or 1800x for new servo position");
  Serial.println("Move the servo to find the required range for whatever you're operating.");
  Serial.println("Servos min and max can vary, try the 1000 - 2000 range to start with.");
  Serial.println("WARNING: Exceeding the max range could damage the servo.");
  Serial.println();
  Servo1.writeMicroseconds(pos);
  Serial.println("Centre point:");  
  Serial.println(pos);  
  delay(10);
}

void loop()
{
  while (Serial.available()) {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }
  if (readString.length() >0) {

    if(readString.indexOf('x') >0) {
      pos = readString.toInt();
    }

    if(readString.indexOf('+') >0) {
      pos = pos + readString.toInt();
    }

    if(readString.indexOf('-') >0) {
      pos = pos - readString.toInt();
    }

    Servo1.writeMicroseconds(pos);
    Serial.println(pos);
    readString=""; //empty for next input
  }
}
