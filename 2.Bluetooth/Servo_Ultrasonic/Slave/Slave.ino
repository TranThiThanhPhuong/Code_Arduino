// Slave
// Servo không sử dụng được với SoftwareSerial.h
// Sử dụng 0,1:
// pin 0 => TX, pin 1 => RX
int command;
// servo
#include <Servo.h>
const int servo_pin = 8;
Servo servo;
int degree = 90;
int servo_count_delay = 0;
int servo_max_delay = 100;
void setup() {
  Serial.begin(9600);

  servo.attach(servo_pin);
}

void loop() {
  if (Serial.available()) {
    command = Serial.read();
    if (command == '0') {
      degree = 90;
    } else if (command == '1') {
      degree = 180;
    }
  }

  if (servo_count_delay == 0) {
    servo.write(0);
  } else if (servo_count_delay == (servo_max_delay >> 1)) {
    servo.write(degree);
  }
  servo_count_delay++;
  if (servo_count_delay > servo_max_delay)
    servo_count_delay = 0;
  delay(10);
}