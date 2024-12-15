// Servo không hoạt động với SoftwareSerial
#include <ArduinoJson.h>
StaticJsonDocument<200> controlJson;
StaticJsonDocument<200> messageJson;
DeserializationError error;
/****************************************
 * Stepper
 */
#include <Servo.h>
const int SERVO_PIN = 12;
Servo servo;

int servo_delay_count = 0;
const int servo_max_delay = 50;
int sensor_delay_count = 0;
const int sensor_max_delay = 10;
//
int vrx, vry;
int degree = 90;
int steps;
/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
}

void loop() {
  if (Serial.available()) {
    error = deserializeJson(controlJson, Serial);
    if (!error) {
      degree = controlJson["degree"].as<int>();
    }
  }
  // do rotate
  if (servo_delay_count == 0) {
    servo.write(degree);
  } else if (servo_delay_count == (servo_max_delay >> 1)) {
    servo.write(0);
  }
  servo_delay_count++;
  if (servo_delay_count == servo_max_delay)
    servo_delay_count = 0;
  // read sensor value

  if (sensor_delay_count == 0) {
    vrx = analogRead(A0);
    vry = analogRead(A1);
    messageJson["x"] = vrx;
    messageJson["y"] = vry;
    serializeJson(messageJson, Serial);
  }
  sensor_delay_count++;
  if (sensor_delay_count == sensor_max_delay)
    sensor_delay_count = 0;

  delay(10);
}
