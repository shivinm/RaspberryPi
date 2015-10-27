/*
 * 
 *  Code to put data 
 *  Serial 9600, 8, N, 1 on USB and first serial port on Teensy (which is 2)
 * 
 * 
 * 
 */

#include "Arduino.h"

HardwareSerial Uart2 = HardwareSerial2();

unsigned long counter=0;

void setup() 
{
  Serial.begin(115200);
  Uart2.begin(115200);

  // Alternative is to use Serial.begin for USB
  // and Serial1.begin for first serial port, which makes
  // more sense...

}

void loop() 
{
  counter++;
  
  Serial.print("Hello Laptop, ");
  Serial.println(counter);
  
  Uart2.print("Hello RaPi, ");
  Uart2.println(counter);
 
  delay(100);

}
