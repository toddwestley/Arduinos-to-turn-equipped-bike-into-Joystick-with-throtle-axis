// code from https://github.com/mockendon/opengradesim/blob/master/OpenGradeSIM_CombinedCode_027.ino
#include <ArduinoBLE.h>
#include <Wire.h>

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
boolean gameOver = false;
long currentRotations;
float speedMPH;
float speed_should_be;
long lastRecordTime;
#define maxSpeed 25.5
#define speedDelta 11.77
#define speedGradient 8.61
#define minThrottle 0.0
#define maxThrottle 1.0
BLEDevice cablePeripheral;
BLECharacteristic speedCharacteristic;
bool firstData = true;
#define  sixty_four_thousand 128000
void setup() {
  Serial.begin(9600);
  if (!BLE.begin()) {
  }
  Wire.begin();
}

void loop() {
  while (!cablePeripheral.connected()) 
  { 
     BLE.scan();
     cablePeripheral = BLE.available();
     if ((cablePeripheral) and !gameOver)
     {
      if ( (cablePeripheral.address() == "d5:1a:de:86:93:e9")) //was >CABLE E5:97:EE:07:27:F2 bike is ==>>D5:1A:DE:86:93:E9 // ddeleted //D5:1A:DE:86:93:E9
      {
        BLE.stopScan();
        getsubscribedtoSensor(cablePeripheral); 
      } 
     } 
     delay(100);
  }  
  long currentMillis = millis(); // line 254
}
void getsubscribedtoSensor(BLEDevice cablePeripheral)
{
  Serial.println(F("Connecting ..."));
  if (cablePeripheral.connect()) 
    {
    }
    else
    { 
      return;
    }
    if (cablePeripheral.discoverService("1816"))
    {
    }
    else
    {
      cablePeripheral.disconnect();   
      return;
    } 
    speedCharacteristic = cablePeripheral.characteristic("2a5B"); //line 322
    if (!speedCharacteristic.subscribe())
    {
    }
    else
    {
    } 
    weightPrevMillis = millis(); // line 277
    int attempts = 0;
    while (!gameOver)
      { refreshSpeedandpower(cablePeripheral); 
        while (millis()-weightPrevMillis<1)
          {  //Serial.print(F("")); 
          }
          if ((!cablePeripheral.connected()) and (attempts < 1))
            { cablePeripheral.connect();
              attempts ++;}
          weightPrevMillis = millis();
      }
      if (gameOver)
      {
        Serial.println(F("in getsubscribedtoSensor and returning "));
        cablePeripheral.disconnect();
        return;  
      }
}

void refreshSpeedandpower(BLEDevice cablePeripheral) // line 382
{
  if (speedCharacteristic.valueUpdated())
  {
      String speed_string;
      float distanceTravelled;
      uint8_t holdvalues[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} ; // line 423
      speedCharacteristic.readValue(holdvalues, 7);
      byte rawValue0 = holdvalues[0];       // binary flags 8 bit int
      byte rawValue1 = holdvalues[1];       // revolutions least significant byte in HEX        
      byte rawValue2 = holdvalues[2];       // revolutions next most significant byte in HEX    
      byte rawValue3 = holdvalues[3];       // revolutions next most significant byte in HEX
      byte rawValue4 = holdvalues[4];       // revolutions most significant byte in HEX
      byte rawValue5 = holdvalues[5];       // time since last wheel event least sig byte in HEX
      byte rawValue6 = holdvalues[6];       // time since last wheel event most sig byte in HEX 
      /*if ((rawValue1 == previousRawValue1) and (millis()-lastRecordTime>sixty_four_thousand)) 
        { Serial.println(F("game over...."));
          gameOver = true;
          return;
        } */
      uint8_t sendValues[8];
      int i;
      for (i = 0; i<=7; i = i +1)
         {  sendValues[i] = holdvalues[i];  
            Serial.print(F(" ,"));
            Serial.print(sendValues[i]);
         }
      Serial.println();          
      Wire.beginTransmission(7); // transmit to device 7 //need to receive this dataa!!
      Wire.write( sendValues,8); //need to receive this data!!
      //Wire.write(sendValues[0]);
      /*Wire.write(sendValues[1]);
      Wire.write(sendValues[2]);
      Wire.write(sendValues[3]);
      Wire.write(sendValues[4]);
      Wire.write(sendValues[5]);
      Wire.write(sendValues[6]);
      Wire.write(sendValues[7]); */
      Wire.endTransmission(); //need to receive this dataa!!
      
      /*Serial.print(rawValue0);
      Serial.print(F(" "));
      Serial.print(rawValue1);
      Serial.print(F(" "));
      Serial.print(rawValue2);
      Serial.print(F(" "));
      Serial.print(rawValue3);
      Serial.print(F(" "));
      Serial.print(rawValue4);
      Serial.print(F(" "));
      Serial.print(rawValue5);
      Serial.print(F(" ")); 
      Serial.print(rawValue6);
      Serial.println(F(" ")); */
      /*if (firstData)
      { currentRotations = (rawValue1 + (rawValue2 * 256) + (rawValue3 * 65536) + (rawValue4 * 16777216));
        WheelRevs1 = (rawValue1 + (rawValue2 * 256) + (rawValue3 * 65536) + (rawValue4 * 16777216));  //line 438
        Time_1 = (rawValue5 + (rawValue6 * 256));
        firstData= 0;
      }
      else
      {
        long WheelRevsTemp = (rawValue1 + (rawValue2 * 256) + (rawValue3 * 65536) + (rawValue4 * 16777216)); //line 447
        long TimeTemp = (rawValue5 + (rawValue6 * 256)); 
         {
            WheelRevs2 = WheelRevsTemp;
            Time_2 = TimeTemp;
            firstData = true;
            distanceTravelled = ((WheelRevs2 - WheelRevs1) * wheelCircCM);
            float kmTravelled = distanceTravelled / 1000000;
            float timeDifference = (Time_2 - Time_1);
            float timeSecs = timeDifference / 1024;
            float timeHrs = timeSecs / 3600;
            speedKMH = (kmTravelled / timeHrs);
            speedMPH = speedKMH * 0.621371;
            double distance_travelled = WheelRevsTemp*wheelCircCM*0.621371/1000/1000;
            //speed_should_be = -exp(-distance_travelled/(8.61)*11.77+25.5);
            speed_should_be = -exp(-distance_travelled/speedGradient)*speedDelta+maxSpeed; 
            speed_should_be  = (speedMPH/speed_should_be *(maxThrottle-minThrottle)+minThrottle);
            if (speed_should_be<minThrottle)
              speed_should_be = minThrottle;
           if (isnan( (speedMPH) == 1) or (speedMPH<0)) // does not work
            speed_should_be = 0;
           if (distance_travelled < 0.10)
            speed_should_be = 0.25+speed_should_be/100;
            Serial.print(speed_should_be);
            Serial.print(F(", "));
            Serial.print(distance_travelled);
            Serial.print(F(", "));
            Serial.print(speedMPH);
            Serial.println(F(","));
         }
      }  
      if (distanceTravelled>0)
        lastRecordTime = millis();*/
      previousRawValue1 = rawValue1;
  } 
} 
