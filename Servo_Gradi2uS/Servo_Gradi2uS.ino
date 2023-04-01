#include <Servo.h>
    
Servo servo1;
int servoPin = 9;
#define USMIN  60 // 300 for MG90S 350 for SG90
#define USMAX  200 // per MG90S e SG90
 
void setup(){
   Serial.begin(9600);
   servo1.attach(servoPin);
}
 
void loop(){
 
   for(int i=USMIN; i < USMAX; i++ ){
      int t = i*10;
      int angle = map(i, USMIN, USMAX, 0, 180);
      servo1.writeMicroseconds(t);
      delay(500);
      Serial.print("Angle: ");
      Serial.print(angle);
      Serial.print(" degrees. (");
      Serial.print(t);
      Serial.println("uS)");
   }
}
