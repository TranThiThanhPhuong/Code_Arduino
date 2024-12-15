// Master: HC-05 của chung
#include <SoftwareSerial.h>
// pin 3 => RX
// pin 2 => TX
SoftwareSerial BTSerial(2, 3); // RX | TX

void setup()
{
  BTSerial.begin(9600);
}

void loop() 
{
  BTSerial.write("0");
  delay(200);
  BTSerial.write("1");
  delay(200);
}