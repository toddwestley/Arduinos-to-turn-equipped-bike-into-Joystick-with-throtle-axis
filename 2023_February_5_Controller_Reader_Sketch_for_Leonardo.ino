
// Wire Controller Reader
// by Nicholas Zambetti <http://www.zambetti.com>
// Demonstrates use of the Wire library
// Reads data from an I2C/TWI peripheral device
// Refer to the "Wire Peripheral Sender" example for use with this
// Created 29 March 2006
// This example code is in the public domain.
//https://docs.particle.io/reference/device-os/api/wire-i2c/onreceive/
//https://docs.arduino.cc/learn/communication/wire
//https://stackoverflow.com/questions/68590764/how-to-properly-use-wire-onreceive-in-arduino
//https://gitee.com/myshitty/ArduinoJoystickLibrary
#include <Wire.h>
#include "Joystick.h" //was <joystick>

volatile bool flag = false;
long weightPrevMillis = 0;        // last time for weight setting
long weightMillis = 0;            // time for weight setting
long WheelRevs1;                  // For speed data set 1
long Time_1;                      // For speed data set 1
long WheelRevs2;                  // For speed data set 2
long Time_2;
int wheelCircCM = 2096;           // Wheel circumference in centimeters (700c 22 road wheel) line 95 was 2350
float speedKMH; //was int
int speedTrainer = 0;             // variable for the speed (kph) read from bluetooth
byte previousRawValue1 = 77;
long currentRotations;
long previous_rotations;
float speedMPH;
float speed_should_be;
long lastRecordTime;
#define maxSpeed 25.5
#define speedDelta 11.77
#define speedGradient 8.61
#define minThrottle -32767 //was  16383 //was -127
#define maxThrottle 32767  //was -16383 //was  127 
double wheel_circumference =(2096);
double miles_per_kilometer= (0.621371);
long int wheel_event_previous;
long int wheel_event_current;
long int delta_wheel_event;
float current_speed;
bool firstData = true;
#define  sixty_four_thousand 64000
float distanceTravelled;
boolean joystickStarted = false;
boolean setZeroDistance = false;
float zeroDistance;

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, 
  0x01, 0, 0, //button count // switch count // was JOYSTICK_TYPE_MULTI_AXIS became 0x01
  false, false, false, false, false, false, //x axis // y axis// z axis// rx axis // ry axis// rz axis
  false, true, false, false, false); //rudder // throttle // accelerator // brake // steering
 
void receiveEvent(int howMany) {
  //Serial.print("in receiveEvent :");
  //Serial.println(howMany);
  uint8_t rawValue[8];
  int spotRawValue = 0;
  double joystickThrottleValue; // was int float
  /*if (not(joystickStarted) and (Wire.available()>1)) //was Wire.available()<1
  {
    joystickStarted = true;  
    Joystick.setThrottleRange(minThrottle,maxThrottle); // per "https://github.com/MHeironimus/ArduinoJoystickLibrary/blob/master/examples/FlightControllerTest/FlightControllerTest.ino"this comes first
    Joystick.begin();
    
  }*/
  while(1 < Wire.available()) { // loop through all but the last
    volatile uint8_t c = Wire.read();       // receive byte
    //Serial.print(c);            // print the byte
    //Serial.print(F(" x "));
    rawValue[spotRawValue] = c;
    spotRawValue ++;
  }
  volatile uint8_t x = Wire.read();          // receive byte as an integer
  rawValue[spotRawValue] = x;
  //Serial.println(x);            // print the integer
  currentRotations = (rawValue[1] + (rawValue[2] * 256) + (rawValue[3] * 65536) + (rawValue[4] * 16777216));
  wheel_event_current = rawValue[5] +  rawValue[6]*256;
   if (firstData)
      { //currentRotations = (rawValue[1] + (rawValue[2] * 256) + (rawValue[3] * 65536) + (rawValue[4] * 16777216));
        WheelRevs1 = (rawValue[1] + (rawValue[2] * 256) + (rawValue[3] * 65536) + (rawValue[4] * 16777216));  
        Time_1 = (rawValue[5] + (rawValue[6] * 256));
        wheel_event_current = Time_1;
        //Serial.println("within first if firstData");
        firstData= 0;
      }
   else
   {
      long WheelRevsTemp = (rawValue[1] + (rawValue[2] * 256) + (rawValue[3] * 65536) + (rawValue[4] * 16777216));
      long TimeTemp = (rawValue[5] + (rawValue[6] * 256));
      WheelRevs2 = WheelRevsTemp;
      Time_2 = TimeTemp;
      //firstData = true; // why??
      distanceTravelled = ((WheelRevs2 - WheelRevs1) * wheelCircCM);
      float kmTravelled = distanceTravelled / 1000000; 
      float timeDifference = (Time_2 - Time_1);
      float timeSecs = timeDifference / 1024;
      float timeHrs = timeSecs / 3600;
      speedKMH = (kmTravelled / timeHrs); //does not take advantage of last event!
      float approximate_speed = (currentRotations-previous_rotations)*wheel_circumference/1000/1000*3600*miles_per_kilometer;
      //Serial.print(F(" current rotation = "));
      //Serial.print(currentRotations);
      //Serial.print(F(" previous rotation = "));
      //Serial.print(previous_rotations);
      if ( wheel_event_current> wheel_event_previous)
        { delta_wheel_event = wheel_event_current - wheel_event_previous;
        }
      else
        { delta_wheel_event = 65536+ wheel_event_current - wheel_event_previous;
        }
      Serial.print(F(" delta wheel event = "));
      Serial.print(delta_wheel_event);  
      current_speed = approximate_speed* 1024/delta_wheel_event;
      
      speedMPH = speedKMH * 0.621371;
      double distance_travelled = WheelRevsTemp*wheelCircCM*0.621371/1000/1000;
      /*if (not setZeroDistance)
      { zeroDistance = distance_travelled;
        setZeroDistance = true; }
      distance_travelled = distance_travelled - zeroDistance;
       */
      speed_should_be = -exp(-distance_travelled/speedGradient)*speedDelta+maxSpeed; 

      
      //Serial.print(F(", speed_cOULD_be "));
      float speed_COULD_be = current_speed/speed_should_be;
      if (speed_COULD_be < 0) speed_COULD_be = 0;
      if (speed_COULD_be > 1) speed_COULD_be = 1;
      
      //speed_COULD_be = map(speed_COULD_be,0,1,minThrottle,maxThrottle); //#define minThrottle -127 //was  16383
      speed_COULD_be = map(speedMPH/speed_should_be,0,1,maxThrottle,minThrottle);
      //Serial.print(speed_COULD_be);
      //speed_should_be  = (current_speed/speed_should_be *(maxThrottle-minThrottle)+minThrottle);
      //if (speed_should_be<minThrottle)
      //        speed_should_be = minThrottle;
      /*if (distance_travelled < 0.10)
            speed_should_be = 0.25*(maxThrottle-minThrottle)+minThrottle; // distance to start engine // was 0.25+speed_should_be/100; */
      
      //speed_should_be = floor(speed_should_be); //was floor
      //speed_should_be = -speed_should_be/2.0;
      //Joystick.setThrottle((int)speed_should_be);
      Serial.print(F("  speed is = "));
      Serial.print(current_speed);
      Serial.print(F("  speed_should_be "));
      Serial.print(speed_should_be);
      Serial.print(F(" distance_travelled== "));
      Serial.print(distance_travelled);
      joystickThrottleValue = (current_speed/speed_should_be);//*(maxThrottle-minThrottle)+minThrottle ) ;
      joystickThrottleValue = joystickThrottleValue*(32767.0+32767.0);
      joystickThrottleValue = joystickThrottleValue - 32767.0;
      /*if (distance_travelled < 0.1)
        { joystickThrottleValue = 0.25*(-32767.0/2.0); }*/
      if (joystickThrottleValue < -32767)
        { joystickThrottleValue = -32767; }
      if (joystickThrottleValue > 32767)
        { joystickThrottleValue  = 32767; } 
      Serial.print(F("  joystick set.throttle == "));
      Serial.print(int(joystickThrottleValue));
      Joystick.setThrottle(int(joystickThrottleValue));
      
     
      Serial.println(F("  X")); 
   }
   wheel_event_previous = wheel_event_current;
   previous_rotations=currentRotations;
}

void setup() {
  //Serial.println(F("serial started1"));
  Wire.begin(7);        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
  Serial.println(F("serial started2"));
  Wire.onReceive(receiveEvent);
  
  Joystick.setThrottleRange(minThrottle,maxThrottle);      // moved to receive event
  Joystick.begin();                                        // moved to receive event
}
void loop() {
  delay(10);
}
