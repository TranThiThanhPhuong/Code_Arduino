// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
// sensor
int value;
//
void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  value = analogRead(A0);
  Serial.println(value);
  if (value <= 600) {
    BTSerial.write('0');
  } else {
    BTSerial.write('1');
  }
  delay(50);
}
