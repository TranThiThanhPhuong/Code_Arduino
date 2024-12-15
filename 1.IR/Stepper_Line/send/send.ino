#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

int value;
const int linePin = 8;

void setup() {
  Serial.begin(9600);
  pinMode(linePin, INPUT);
}

void loop() {
  value = digitalRead(linePin);
  Serial.println(value);

  if (value == LOW)
    irsend.sendNEC(0x00, 0x1A, noRepeats);
  else
    irsend.sendNEC(0x00, 0x1B, noRepeats);
  
  delay(100);
}