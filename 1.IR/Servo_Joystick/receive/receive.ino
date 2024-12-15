#include <IRremote.h>
#define IR_RECEIVE_PIN 2
#define IR_VCC_PIN 3
#define IR_GND_PIN 4
//
uint32_t command;
//
#include <Servo.h>
const int servo_pin = 12;
Servo servo;
int degree = 90;
int servo_count_delay = 0;
int servo_max_delay = 100;

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);  // Start the receiver
  pinMode(IR_VCC_PIN, OUTPUT);
  pinMode(IR_GND_PIN, OUTPUT);
  digitalWrite(IR_VCC_PIN, HIGH);
  digitalWrite(IR_GND_PIN, LOW);
  //
  servo.attach(servo_pin);
}

void loop() {
  if (IrReceiver.decode()) {
    command = IrReceiver.decodedIRData.command;
    Serial.print("Received command: ");
    Serial.println(command, HEX);
    //
    if (command == 0x1A) {
      degree = 45;
    } else if (command == 0x1B) {
      degree = 90;
    } else if (command == 0x1C) {
      degree = 135;
    } else if (command == 0x1D) {
      degree = 180;
    }
    //
    IrReceiver.resume();  // Enable receiving of the next value
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