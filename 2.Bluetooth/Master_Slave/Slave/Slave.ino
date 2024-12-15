// Slave: HC-05 của Minh
// ADDR:0022:12:018E86
#include <SoftwareSerial.h>
// pin 3 => RX
// pin 2 => TX
SoftwareSerial BTSerial(2, 3); // RX | TX

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() 
{
  if (BTSerial.available()) {
    int code = BTSerial.read();
    Serial.print("Receive: ");
    Serial.println(code);
    if (code == '0') {
      Serial.println("Hello World!");
    } else if (code == '1') {
      Serial.println("Goodbye World!");
    }
  }
}