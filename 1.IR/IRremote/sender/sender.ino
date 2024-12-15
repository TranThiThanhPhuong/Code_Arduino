#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

void setup() {
}


void loop() {
  irsend.sendNEC(0x00, 0x1C, noRepeats);
  delay(1000);
  irsend.sendNEC(0x00, 0x1D, noRepeats);
  delay(1000);
}