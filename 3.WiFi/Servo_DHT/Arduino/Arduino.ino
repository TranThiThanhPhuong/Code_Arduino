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
const int servo_max_delay = 100;
int sensor_delay_count = 0;
const int sensor_max_delay = 10;
//
#include <DHT.h>
const int dht11_pin = 11;
const int dht_model = DHT11;
float humidity;
float temperature;
DHT dht11(dht11_pin, dht_model);
//
int degree = 90;

/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  servo.attach(SERVO_PIN);
  dht11.begin();
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
    humidity = dht11.readHumidity();
    temperature = dht11.readTemperature();
    messageJson["humidity"] = humidity;
    messageJson["temperature"] = temperature;
    serializeJson(messageJson, Serial);
  }
  sensor_delay_count++;
  if (sensor_delay_count == sensor_max_delay)
    sensor_delay_count = 0;

  delay(10);
}
