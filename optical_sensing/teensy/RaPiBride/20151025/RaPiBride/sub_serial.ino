
void SerialInputProcess(void)
/////////////////////////////////////////////////////////////////////////////////////////////
// Serial is the USB port on the Teensy
// Serial1 is the regualr serial port on pins 0 and 1
// Here, anything coming from the Teensy USB port is sent to the RPi via Serial1, on pins 0 and 1
{
 
  if (Serial.available()) {
    // Read a line from Serial port
    Serial.setTimeout(100); // 100 millisecond timeout
    String s = Serial.readString();

    // send to Serial1
    Serial1.print(s);
   
  }

}

void SerialInputProcess1(void)
/////////////////////////////////////////////////////////////////////////////////////////////
// Serial is the USB port on the Teensy
// Serial1 is the regualr serial port on pins 0 and 1
// Here, anything coming from the RPi is sent to Serial (the Teensy USB port)
{
  String cmd;
  String cmd_name;
  String cmd_subname;
  String cmd_value;

  if (Serial1.available()) {
    // Read a line from Serial1 port
    Serial1.setTimeout(100); // 100 millisecond timeout
    String s = Serial1.readString();

    // send to Serial1
    Serial.print(s);
  }
}

