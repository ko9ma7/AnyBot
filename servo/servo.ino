/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

String servoMotorIndex;
String str2;
String str3;
String readString;

void setup() {
  Serial.begin(115200);
  pwm.begin();

  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
  Serial.print(pulselength); Serial.println(" us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println(" us per bit"); 
  pulse *= 1000000;  // convert input seconds to us
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

void loop() {
  recvWithEndMarker();
}

void recvWithEndMarker() {

  while(!Serial.available()); // wait till data to be filled in serial buffer

  String readString = Serial.readStringUntil('#'); // read the complete string

  servoMotorIndex = readString.substring(0, readString.indexOf(" "));
  //str2 = readString.substring(readString.indexOf(" ") + 1);
  //str3 = readString.substring(str2.indexOf(" ") + 1);
  
  int frase[4], r=0, t=0;
    
  for (int i=0; i < readString.length(); i++)
  { 
   if(readString.charAt(i) == ' ') 
    { 
      frase[t] = readString.substring(r, i).toInt(); 
      r=(i+1); 
      t++; 
    }
  }

  doRotation(frase[0], frase[1], frase[2], frase[3]);

  //delay(500);
}

void doRotation(int pin, int initial, int goal, int delayValue)
{
  if(initial < goal) 
    { 
      for (int pulselen = initial; pulselen < goal; pulselen++)
        {
          pwm.setPWM(pin, 0, pulselen);
          delay(delayValue);  //  <-------- Increasing this delay will slow down the servo movement
        } 
    }
    
  if(initial > goal) 
    { 
      for (int pulselen = initial; pulselen > goal; pulselen--)
        {
          pwm.setPWM(pin, 0, pulselen);
          delay(delayValue);  //  <-------- Increasing this delay will slow down the servo movement
        } 
    }
  
}
