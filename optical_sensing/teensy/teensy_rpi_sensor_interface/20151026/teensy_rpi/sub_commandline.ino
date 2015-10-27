
void SerialInputProcess(void)
/////////////////////////////////////////////////////////////////////////////////////////////
// This is for processing the Arduino IDE serial window
// which operates differently than other terminal programs
// If you need to use other terminal programs, see SerialInputProcess1, which handles
// characters one at a time.
{
  String cmd;
  String cmd_name;
  String cmd_subname;
  String cmd_value;

  if (Serial.available()) {
    // Read a line from Serial port
    Serial.setTimeout(100); // 100 millisecond timeout
    String s = Serial.readString();

    // We need to convert the line to bytes, no more than 20 at this time
    uint8_t sendbuffer[20];
    s.getBytes(sendbuffer, 20);
    char sendbuffersize = min(20, s.length());

    // GenericSerialPrint(F("\n* Sending -> \"")); GenericSerialPrint((char *)sendbuffer); GenericSerialPrintln("\"");

    // process command, spaces are delimiters
    // c x 1.2345

    int SpaceIndex = s.indexOf(' ');
    int secondSpaceIndex = s.indexOf(' ', SpaceIndex + 1);

    cmd_name = s.substring(0, SpaceIndex);
    cmd_subname = s.substring(SpaceIndex + 1, secondSpaceIndex);
    cmd_value = s.substring(secondSpaceIndex);
    ProcessCommand(cmd_name, cmd_subname, cmd_value);
    cmd = "";

  }

}

void SerialInputProcess1(void)
/////////////////////////////////////////////////////////////////////////////////////////////
{
  String cmd;
  String cmd_name;
  String cmd_subname;
  String cmd_value;

  if (Serial1.available()) {
    // Read a line from Serial port
    Serial1.setTimeout(100); // 100 millisecond timeout
    String s = Serial1.readString();

    // We need to convert the line to bytes, no more than 20 at this time
    uint8_t sendbuffer[20];
    s.getBytes(sendbuffer, 20);
    char sendbuffersize = min(20, s.length());

    // We have to concatenate from Serial1
    // The regular Serial port, using the Arduino IDE sends the command after CR is hit, which is slightly
    // different than other terminal programs.
    // This is different than the RPI putty, which sends characters immediately after you put them in.

    /*
    int temp=0;
    temp=sendbuffersize;

    Serial.print("sendbuffer:");
    Serial.print(sendbuffersize);

    Serial.print(" ascii=");
    Serial.print(temp);
    Serial.print(" sendbuffer[0]=");
    Serial.print(sendbuffer[0]);
    Serial.print(" sendbuffer[0]=");
    Serial.print("\n");

    Serial.print("carriage return = ");
    Serial.print(int('\n'));
    Serial.print("\n");

    Serial.print("carriage return = ");
    Serial.print(int('\r'));
    Serial.print("\n");

    Serial.print(" char(sendbuffer)=");
    Serial.print(char(sendbuffer[0]));
    Serial.print("\n");

    */

    Serial1String = Serial1String + char(sendbuffer[0]);

    /*
    Serial.print("Serial1String = ");
    Serial.print(Serial1String);
    Serial.print("\n");
    */

    // GenericSerialPrint(F("\n* Sending -> \"")); GenericSerialPrint((char *)sendbuffer); GenericSerialPrintln("\"");

    // process command, spaces are delimiters
    // c x 1.2345

    // a lot depends on the terminal program used, you could be sending carriage return
    // or line feed, so we'll assume both are sent...
    if (sendbuffer[0] == '\n' || sendbuffer[0] == '\r' )
    {

      // s set to Serial1String
      s = Serial1String;

      /*
      Serial.print("Serial1String processing = ");
      Serial.print(s);
      Serial.print("\n");
      */

      int SpaceIndex = s.indexOf(' ');
      int secondSpaceIndex = s.indexOf(' ', SpaceIndex + 1);

      cmd_name = s.substring(0, SpaceIndex);
      cmd_subname = s.substring(SpaceIndex + 1, secondSpaceIndex);
      cmd_value = s.substring(secondSpaceIndex);
      ProcessCommand(cmd_name, cmd_subname, cmd_value);
      cmd = "";

      // clear concatenated string
      Serial1String = "";

    }

  }
}

void ProcessCommand(String comletter, String comletter2, String inputvalue)
/////////////////////////////////////////////////////////////////////////////////////////////
// Process commands from serial port
// two commands and argument
//
{
  char inval[2];
  char inval2[2];
  int key;
  int key2;
  float invalue;
  boolean bSerial = false;
  boolean bServo = false;
  int delta;

  /*
  GenericSerialPrint("comletter=");
  GenericSerialPrint(comletter);
  GenericSerialPrint(" inputvalue=");
  GenericSerialPrintln(invalue);
  */

  // convert to integer to process (for switch)
  invalue = inputvalue.toFloat();
  comletter.toCharArray(inval, 2);
  key = inval[0];

  comletter2.toCharArray(inval2, 2);
  key2 = inval2[0];

  //GenericSerialPrintln(inval);
  //GenericSerialPrintln(key);

  switch (key) // .....................................................
  {

    case 'h': // help

      GenericSerialPrintln(F(" "));
      GenericSerialPrintln(F("help on commands:"));
      GenericSerialPrintln(F(" "));
      GenericSerialPrintln(F("?          = info on program"));
      GenericSerialPrintln(F("p          = poll one data line"));
      GenericSerialPrintln(F("s f        = serial interval print off"));
      GenericSerialPrintln(F("s o        = serial interval print on"));
      GenericSerialPrintln(F("s i ZZZZ   = serial interval print every ZZZZ ms"));
      GenericSerialPrintln(F("s h        = show field header list"));
      GenericSerialPrintln(F("m a i      = servo motor to absolute position i"));
      GenericSerialPrintln(F("m + d      = servo motor relative + d"));
      GenericSerialPrintln(F("m - d      = servo motor relative - d"));
      
      GenericSerialPrintln(F(" "));

      break;

    case '?': // info
      GenericSerialPrintln(F("Teensy/RPI interface."));
      GenericSerialPrint(F("Version:"));
      GenericSerialPrintln(String(VERSIONID));
      GenericSerialPrintln("h for help on commands.");
      break;

    case 's': // serial print
      bSerial = true;
      break;

    case 'p': // poll single data line
      // GenericSerialPrint(SingleDataLineHeader());
      GenericSerialPrint(SingleDataLine());
      break;

    case 'm':
      bServo = true;
      break;

  }

  switch (key2) // .....................................................
  {

    case 'h': // show header
      if (bSerial == true) {
        GenericSerialPrint(SingleDataLineHeader());
      }
      break;

    case 'i': // interval, increment
      if (bSerial == true) {
        GenericSerialPrint("dump interval set = ");
        Interval_Print = int(invalue);
        Interval_Print = constrain(Interval_Print, 0, 5000);
        GenericSerialPrintln(String(Interval_Print));
      }
      break;

    case 'f': // serial off
      SerialDump = false;
      break;

    case 'o': // serial on
      SerialDump = true;
      break;

    case 'a':
      if (bServo == true)
      {
        ServoPosition = int(invalue);
        myservo.write(ServoPosition);
      }
      break;

    case '+':
      if (bServo == true)
      {
        delta = int(invalue);
        ServoPosition = ServoPosition + delta;
        myservo.write(ServoPosition);
      }
      break;

    case '-':
      if (bServo == true)
      {
        delta = int(invalue);
        ServoPosition = ServoPosition - delta;
        myservo.write(ServoPosition);
      }
      break;

  }

}

String SingleDataLineHeader(void)
/////////////////////////////////////////////////////////////////////////////////////////////
{
  String Temp;
  Temp =
    String("CurrentMillis") +  "," +
    String("ServoPosition") +  "," +
    String("tmp007_objt (C)") +  "," +
    String("tmp007_diet (C)") +  "," +
    String("raw_value_distance") + "," +
    String("voltage_distance") + "," +
    String("raw_value_light") + "," +
    String("voltage_light") + "," +
    String("value_distance_mm") + "," +
    "\n";
  return Temp;

}
String SingleDataLine(void)
/////////////////////////////////////////////////////////////////////////////////////////////
{

  String Temp;

  Temp =
    String(CurrentMillis) +  "," +
    String(ServoPosition) +  "," +
    String(tmp007_objt) + "," +
    String(tmp007_diet) + "," +
    String(raw_value_distance, 0) + "," +
    String(voltage_distance, 3) + "," +
    String(raw_value_light, 0) + "," +
    String(voltage_light, 3) + "," +
    StringFormattedFloat(value_distance_mm, 5, 1) + "," +
    "\n";

  return Temp;
}

/////////////////////////////////////////////////////////////////////////////////////////////
String StringFormattedFloat(float fValue, int Width, int Decimals)
{
  char TempValue[20];
  String Temp;

  dtostrf(fValue, Width, Decimals, TempValue);
  Temp = TempValue;
  Temp.trim();
  return Temp;

}
