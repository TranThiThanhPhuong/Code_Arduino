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
const int trigger_pin = 11;
const int pw_pin = 10;
const int trigger_delay = 10;
long duration;
float cm;
//
int degree = 90;
/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  pinMode(trigger_pin, OUTPUT);
  pinMode(pw_pin, INPUT);
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
