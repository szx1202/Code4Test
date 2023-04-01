#include <Wire.h>
#include <Servo.h> //Inserire la libreria Servo

Servo Servo1; //Il nome del servo è Servo1
#define USMIN  350 // 300 for MG90S 350 for SG90
#define USMAX  2600 // per MG90S e SG90
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 1; // address of servo to be calibrated
void setup() {
  Servo1.attach (9); //Il Servo1 è collegato al pin digitale 
}

void loop() {

  Servo1.writeMicroseconds(350);
  delay (500);
  Servo1.writeMicroseconds(2600);
  delay (500);
}
