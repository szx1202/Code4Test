// ver 1.0 --> first build
// Calibrate servo connected with a PCA9685 to and Arduino +6 HC-06 Blutooth Module
// controlled via Bluetooth using an app, developed with  MIT App Inventory, for Android devices 

#include <SoftwareSerial.h>
#define BT_TX_PIN 4 //define pin TX from Arduino to BT Module
#define BT_RX_PIN 2 ///define pin RX from Arduino to BT Module
SoftwareSerial bluetooth(BT_RX_PIN, BT_TX_PIN );

#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
//SDA (default is GPIO 21) SCL (default is GPIO 22)

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x42); //ALWAYS CHECK PCA9685 ADDRESS
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

uint8_t servonum = 15; // address of servo to be calibrated
String readString;
int pos;

//https://www.makerslab.it/pca9685-controllo-i2c-a-16-canali-pwm-per-led-e-servomotori/
//#define SERVOMIN  150 //set 150 300  this is the 'minimum' pulse length count (out of 4096)
//#define SERVOMAX  185 //set 175 600  this is the 'maximum' pulse length count (out of 4096)

void setup()
{
// Bluetooth pin initialization
//  pinMode(BT_RX_PIN, INPUT);
//  pinMode(BT_TX_PIN, OUTPUT);
  bluetooth.begin(9600); //set baud rate
  Serial.begin(9600); 
  
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
  bluetooth.flush();
  delay(200);
}

void loop()
{
  //Read from bluetooth and write to usb serial 
  while (bluetooth.available()>0)
  {    
    char c = bluetooth.read();
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character

   }
      
   if (readString.length() >0) {

    if(readString.indexOf('x') >0) {
      pos = readString.toInt();
      Serial.println(pos);
      bluetooth.println(pos);
    }

    if(readString.indexOf('+') >0) {
     pos = pos + readString.toInt();
     Serial.println(pos);
     bluetooth.println(pos);

    }

    if(readString.indexOf('-') >0) {
      pos = pos - readString.toInt();
      Serial.println(pos);
      bluetooth.println(pos);
     }
    
    if(readString.indexOf('r') >0) {
      Serial.println(pos);
      bluetooth.println(pos);
    }

    
    
    pwm.writeMicroseconds(servonum, pos);
    readString=""; //empty for next input
    bluetooth.flush();
    delay(200);
    
  }
}
