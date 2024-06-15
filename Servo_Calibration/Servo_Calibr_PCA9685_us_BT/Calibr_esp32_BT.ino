// ver 1.0 --> first build
// Calibrate servo connected with a PCA9685 to and ESP32
// controlled via Bluetooth using an app, developed with  MIT App Inventory, for Android devices 

#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
//SDA (default is GPIO 21) SCL (default is GPIO 22)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 0; // address of servo to be calibrated
String readString;
int pos;

//https://www.makerslab.it/pca9685-controllo-i2c-a-16-canali-pwm-per-led-e-servomotori/
//#define SERVOMIN  150 //set 150 300  this is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  185 //set 175 600  this is the 'maximum' pulse length count (out of 4096)

void setup()
{
  //Setup usb serial connection to computer
  Serial.begin(115200);  
  SerialBT.begin("ESP32Test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
 
 pos=1600; // Initial position
 pwm.setOscillatorFrequency(27000000);
 pwm.begin();
 delay(10);
 pwm.setPWMFreq(SERVO_FREQ);
 
 Serial.println("Servo calibration");
 Serial.println("Use this to calibrate your servo to find the range of movement required");
 Serial.println("The servo should start close to the centre of the range");
 Serial.println("Type a value followed by a + to move in one direction or a valu followed by a - to move in the other direction");
 Serial.println("For example 100+ to 200-");  
 Serial.println("To move to a specific location use strings like 900x or 1800x for new servo position");
 Serial.println("Move the servo to find the required range for whatever you're operating.");
 Serial.println("Servos min and max can vary, try the 500 - 2700 range to start with.");
 Serial.println("WARNING: Exceeding the max range could damage the servo.");
 Serial.println();
 pwm.writeMicroseconds(servonum, pos);
  
 Serial.println("Centre point:");  
 Serial.println(pos);  
 SerialBT.flush();
  delay(200);
 //SerialBT.print(pos);
}

void loop()
{
  //Read from bluetooth and write to usb serial 
  while (SerialBT.available())
  {    
    char c = SerialBT.read();
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character

   }
      
   if (readString.length() >0) {

    if(readString.indexOf('x') >0) {
      pos = readString.toInt();
      Serial.println(pos);
      SerialBT.println(pos);
    }

    if(readString.indexOf('+') >0) {
     pos = pos + readString.toInt();
     Serial.println(pos);
     SerialBT.println(pos);

    }

    if(readString.indexOf('-') >0) {
      pos = pos - readString.toInt();
      Serial.println(pos);
      SerialBT.println(pos);
     }
    
    if(readString.indexOf('r') >0) {
      Serial.println(pos);
      SerialBT.println(pos);
    }

    
    
    pwm.writeMicroseconds(servonum, pos);
    readString=""; //empty for next input
    SerialBT.flush();
    delay(200);
    
  }
}
