#include <ArduinoJson.h>
StaticJsonDocument<200> controlJson;
StaticJsonDocument<200> messageJson;
DeserializationError error;
/****************************************
 * Stepper
 */
#include <Stepper.h>
#define STEPS 2048  // 32 steps * gear reduction ratio 64
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
const int speed = 10;  // [5, 15]
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);
int stepper_delay_count = 0;
const int stepper_max_delay = 50;
int sensor_delay_count = 0;
const int sensor_max_delay = 10;
//
const int trigger_pin = 8;
const int pw_pin = 7;
const int trigger_delay = 10;
long duration;
float cm;
//
int degree = 30;
int steps;
/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  stepper.setSpeed(speed);
}

void loop() {
  // Receive control step
  if (Serial.available()) {
    error = deserializeJson(controlJson, Serial);
    if (!error) {
      degree = controlJson["degree"].as<int>();
    }
  }
  // do rotate
  if (stepper_delay_count == 0) {
    steps = degree * 1.0 * STEPS / 360;
    stepper.step(steps);
  }
  stepper_delay_count++;
  if (stepper_delay_count == stepper_max_delay)
    stepper_delay_count = 0;
  
  // read sensor value
  if (sensor_delay_count == 0) {
    digitalWrite(trigger_pin, LOW);
    delayMicroseconds(10);
    digitalWrite(trigger_pin, HIGH);
    delayMicroseconds(trigger_delay);
    digitalWrite(trigger_pin, LOW);
    duration = pulseIn(pw_pin, HIGH);
    cm = duration * 0.017;
    messageJson["sensor_value"] = cm;
    serializeJson(messageJson, Serial);
  }
  sensor_delay_count++;
  if (sensor_delay_count == sensor_max_delay)
    sensor_delay_count = 0;

  delay(10);
}
