// Master
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
// sensor
int vrx, vry;
//
void setup() {
  BTSerial.begin(9600);
  Serial.begin(9600);
}

void loop() {
  vrx = analogRead(A0);
  vry = analogRead(A1);
  Serial.print("x: ");
  Serial.print(vrx);
  Serial.print(", y: ");
  Serial.print(vry);
  Serial.println();
  if (vrx <= 200) {
    // Gạt xuống
    BTSerial.write('0');
  } else if (vrx >= 800) {
    // Gạt lên
    BTSerial.write('1');
  }
  if (vry <= 200) {
    // Gạt trái: 
    BTSerial.write('2');
  } else if (vry >= 800) {
    // Gạt phải: 
    BTSerial.write('3');
  }
  delay(50);
}
