// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
// sensor
const int linePin = 8;
int value;
//
void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);
  pinMode(linePin, INPUT);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);
  if (value == LOW) {
    BTSerial.write('0');
  } else {
    BTSerial.write('1');
  }
  delay(50);
}
