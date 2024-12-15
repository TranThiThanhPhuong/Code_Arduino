// Slave
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2, 3);
int command;
// DC
const int en12_pin = 5;
const int dir_1a_pin = 6;
const int dir_2a_pin = 7;
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

void setup() {
  //
  BTSerial.begin(9600);
  Serial.begin(9600);

  pinMode(en12_pin, OUTPUT);
  pinMode(dir_1a_pin, OUTPUT);
  pinMode(dir_2a_pin, OUTPUT);
  //
  digitalWrite(dir_1a_pin, LOW);
  digitalWrite(dir_2a_pin, LOW);
  analogWrite(en12_pin, 0);
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Received command: ");
    Serial.println(command);
    if (command == '0') {
      CW = true;
      faster = true;
    } else if (command == '1') {
      CW = false;
      faster = false;
    }
  }
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
  } else if (dc_count == SPEED_2_LOWERBOUND && faster || dc_count == SPEED_2_UPPERBOUND && !faster) {
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