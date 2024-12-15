// Slave
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);
int command;
// Step
#include <Stepper.h>
#define STEPS 2048  // 32 steps * gear reduction ratio 64
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
const int speed = 15;  // [5, 15]
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);
int degree = 90;
float steps;
int step_count_delay = 0;
int step_max_delay = 50;
//
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  stepper.setSpeed(speed);
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received command: ");
    Serial.println(command);
    if (command == '0') {
      degree = 90;
    } else if (command == '1') {
      degree = -180;
    }
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