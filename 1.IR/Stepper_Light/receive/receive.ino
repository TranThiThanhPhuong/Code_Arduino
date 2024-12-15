#include <IRremote.h>
#define IR_RECEIVE_PIN 2
#define IR_VCC_PIN 3
#define IR_GND_PIN 4
uint32_t command;
// Stepper
#include <Stepper.h>
#define STEPS 2048
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
const int speed = 10; // [5, 15]
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);
int degree = 45;
float steps;
int step_count_delay = 0;
int step_max_delay = 50;
//
void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN);  // Start the receiver
  pinMode(IR_VCC_PIN, OUTPUT);
  pinMode(IR_GND_PIN, OUTPUT);
  digitalWrite(IR_VCC_PIN, HIGH);
  digitalWrite(IR_GND_PIN, LOW);
  stepper.setSpeed(speed);
}

void loop() {
  if (IrReceiver.decode()) {
    command = IrReceiver.decodedIRData.command;
    Serial.print("Received command: ");
    Serial.println(command, HEX);
    if (command == 0x1A)
      degree = 45;
    else if (command == 0x1B)
      degree = -90;
    IrReceiver.resume();
  }

  if (step_count_delay == 0) {
    steps = degree * 1.0 * STEPS / 360;
    stepper.step(steps);
  }
  step_count_delay++;
  if (step_count_delay > step_max_delay)
    step_count_delay = 0;
  delay(10);
}
