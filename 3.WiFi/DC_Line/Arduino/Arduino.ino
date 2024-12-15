#include <ArduinoJson.h>
StaticJsonDocument<200> controlJson;
StaticJsonDocument<200> messageJson;
DeserializationError error;
/****************************************
 * DC
 */
const int en12_pin = 5;
const int dir_1a_pin = 4;
const int dir_2a_pin = 3;
// faster == true: nhanh dần
// faster_cache == false: chậm dần
bool faster = true;
bool faster_cache = false;
// CW == true: CW
// CW == false: CCW
int CW = true;
int CW_cache = false;
int dc_count = 0;
#define SPEED_1 0
#define SPEED_2 190
#define SPEED_3 255
#define SPEED_1_LOWERBOUND 1
#define SPEED_1_UPPERBOUND 200
#define SPEED_2_LOWERBOUND 201
#define SPEED_2_UPPERBOUND 400
#define SPEED_3_LOWERBOUND 401
#define SPEED_3_UPPERBOUND 600
// sensor
const int linePin = 8;
int sensor_delay_count = 0;
const int sensor_max_delay = 5;
int sensor_value = 0;
/****************************************
 * Main
 */
void setup() {
  Serial.begin(115200);
  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, LOW);
  analogWrite(en12_pin, 0);
  pinMode(linePin, INPUT);
}

void loop() {
  /************************************
   *   read sensor value and send message
   */
  if (sensor_delay_count == 0) {
    sensor_value = digitalRead(linePin);
    messageJson["sensor_value"] = sensor_value;
    serializeJson(messageJson, Serial);
  }
  sensor_delay_count++;
  if (sensor_delay_count == sensor_max_delay)
    sensor_delay_count = 0;
  // Listening for DC control Message
  if (Serial.available()) {
    error = deserializeJson(controlJson, Serial);
    if (!error) {
      CW = controlJson["CW"].as<bool>();
      faster = controlJson["faster"].as<bool>();
    }
  }
  /************************************
   *   DC motor control
   */
  // CW <=> CCW
  if (CW != CW_cache) {
    // brake
    digitalWrite(dir_2a_pin, LOW);
    digitalWrite(dir_1a_pin, LOW);
    analogWrite(en12_pin, 0);
    CW_cache = CW;
    faster_cache = !faster;
  }
  // Nhanh dần <=> Chậm dần
  // reset dc_count
  if (faster != faster_cache) {
    faster_cache = faster;
    if (faster)
      dc_count = SPEED_1_LOWERBOUND;  // Nhanh dần
    else
      dc_count = SPEED_3_UPPERBOUND;  // Chậm dần
  }
  // choose speed to rotate
  if (dc_count == SPEED_1_LOWERBOUND && faster || dc_count == SPEED_1_UPPERBOUND && !faster) {
    digitalWrite(dir_1a_pin, LOW);
    digitalWrite(dir_2a_pin, LOW);
    analogWrite(en12_pin, SPEED_1);
  }
  else if (dc_count == SPEED_2_LOWERBOUND && faster || dc_count == SPEED_2_UPPERBOUND && !faster) {
    if (faster) {
      if (CW) digitalWrite(dir_1a_pin, HIGH);
      else digitalWrite(dir_2a_pin, HIGH);
    }
    analogWrite(en12_pin, SPEED_2);
  } else if (dc_count == SPEED_3_LOWERBOUND && faster || dc_count == SPEED_3_UPPERBOUND && !faster) {
    if (!faster) {
      if (CW) digitalWrite(dir_1a_pin, HIGH);
      else digitalWrite(dir_2a_pin, HIGH);
    }
    analogWrite(en12_pin, SPEED_3);
  }
  // update dc_count
  if (faster) {
    ++dc_count;
    if (dc_count > SPEED_3_UPPERBOUND) dc_count = SPEED_1_LOWERBOUND;
  } else {
    --dc_count;
    if (dc_count < SPEED_1_LOWERBOUND) dc_count = SPEED_3_UPPERBOUND;  //reset speed
  }

  delay(10);
}
