
#define VERSIONID "20151026"

//............................................................................................
/*
 * Program to handle various sensors for the Optical Sensing for Robotics and UAVs class
 * at Artisan's Asylum, Fall 2015
 * 
 * Sensors are:
 * distance sensor, on A0: Sharp GP2Y0A21YK0F
 * light sensor, on A1: Adafruit GA1A12S202
 * non-contact temperature, via I2C: Adafruit TMP007 Sensor Breakout
 * 
 * Servo is a basic hobby servo; brown=GND; red=VCC; tan=signal
 * 
 * This is basic Arduino code; no fancy Teensy stuff (like simultaneous analog reads) 
 * has been incorporated.
 * 
 */
 


#include <Wire.h>
#include "Adafruit_TMP007.h"
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

unsigned long previousMillis_Print = 0;
unsigned long CurrentMillis=0;

// I2C temperature
Adafruit_TMP007 tmp007;
float tmp007_objt;
float tmp007_diet;

// analog inputs
#define ANALOG_READ_DELAY 10 // 10 ms delay


// analog inputs; currently only A used
#define AI_LIGHT_A_PIN 0
#define AI_DISTANCE_A_PIN 1

#define AI_LIGHT_B_PIN 2
#define AI_DISTANCE_B_PIN 3

#define AI_LIGHT_C_PIN 4
#define AI_DISTANCE_C_PIN 5

float raw_value_distance;
float raw_value_light;
float voltage_distance;
float voltage_light;
float value_distance_mm;

// servo output; currently only A used
#define SERVO_A_PIN 5
#define SERVO_B_PIN 6

int ServoPosition=0;

// depends if we wait to start
#define WAIT_TO_START    0 // Wait for serial input in setup();0=no wait, 1=wait
long Interval_Print = 1000;

boolean SerialDump = true;
boolean UseSerial = true;
boolean UseSerial1 = true;
boolean tmp007_available;

String SerialString;
String Serial1String;

void setup()
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
{

  // the USB port is here
  Serial.begin(9600);
    
  // This is for the first Serial port (Serial1) on the Teensy
  // GND
  // Pin 1 = RX1
  // Pin 2 = TX1
  Serial1.begin(115200);

  delay(2000);
  
  Serial.println("STARTING...");
  // Wire begin; for TMP007, this is done automatically in the 'begin' routine
  // Wire.begin();

#if WAIT_TO_START
  Serial.println("Type any character to start");
  while (!Serial.available());
#endif //WAIT_TO_START

// TMP007_CFG_1SAMPLE
// default (no argument) is TMP007_CFG_16SAMPLE

Serial.println("Searching for tmp007...");
if (! tmp007.begin(TMP007_CFG_1SAMPLE)) {
    Serial.println("No sensor found");
    tmp007_available=false;
  }
else
  {
  tmp007_available=true;
  }

   myservo.attach(SERVO_A_PIN);  // attaches the servo on pin 9 to the servo object
   myservo.write(ServoPosition); 
   
 // SERIAL OUTPUT
  Serial.println("");
  Serial.print("Teensy-RPI System Version: "); Serial.print(VERSIONID); Serial.println("");

  analogReadResolution(12);
  
}

void loop()
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
{

  CurrentMillis=millis();

  // get inputs
  GetInputValues();

  // process inputs from serial line
  SerialInputProcess();
  SerialInputProcess1();

  // Print out to serial port if required
  if (SerialDump == true)
  {
    SerialPrint();
  }

  // system outputs
  SetSystemOutputs();
 
}


/////////////////////////////////////////////////////////////////////////////////////////////
void SetSystemOutputs(void)
{
   
}



void SerialPrint(void)
/////////////////////////////////////////////////////////////////////////////////////////////
{
  
  if ((CurrentMillis - previousMillis_Print) >= Interval_Print) {

    GenericSerialPrint(SingleDataLine());
    previousMillis_Print = CurrentMillis;
    
  }

}


void GetInputValues(void)
/////////////////////////////////////////////////////////////////////////////////////////////
{

  // read digital and analog inputs

  
  delay(ANALOG_READ_DELAY);
  
  raw_value_distance  = analogRead(AI_DISTANCE_A_PIN);
  voltage_distance = 3.3*(raw_value_distance/4095);
  value_distance_mm = 27 * (1.0/voltage_distance) * 10.0;

  delay(ANALOG_READ_DELAY);
  raw_value_light      = analogRead(AI_LIGHT_A_PIN);
  voltage_light = 3.3*(raw_value_light/4095);

  // Temperature sensor, I2C
  if(tmp007_available==true)
  {
  tmp007_objt = tmp007.readObjTempC();
  tmp007_diet = tmp007.readDieTempC();
  }

}

void GenericSerialPrint(String InputString)
/////////////////////////////////////////////////////////////////////////////////////////////
{

  // This will print to either Serial, or Serial1, depending on what has been requested.

  if (UseSerial == true)
  {
    Serial.print("SERIAL," + InputString);
  }
  if (UseSerial1 == true)
  {
    Serial1.print("SERIAL1," + InputString);
  }

}

void GenericSerialPrintln(String InputString)
/////////////////////////////////////////////////////////////////////////////////////////////
{

  // This will print to either Serial, or Serial1, depending on what has been requested.

  if (UseSerial == true)
  {
    Serial.println("SERIAL," + InputString);
  }
  if (UseSerial1 == true)
  {
    Serial1.println("SERIAL1," + InputString);
  }

}
