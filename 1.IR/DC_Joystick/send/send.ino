#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

int vrx, vry;

void setup() {
  Serial.begin(960);
}

void loop() {
  vrx = analogRead(A0);
  vry = analogRead(A1);
  Serial.print("(");
  Serial.print(vrx);
  Serial.print(", ");
  Serial.print(vry);
  Serial.println(")");


  if (vrx <= 300)
    irsend.sendNEC(0x00, 0x1A, noRepeats);
  else if (vrx >= 700)
    irsend.sendNEC(0x00, 0x1B, noRepeats);
  else if (vry <= 300)
    irsend.sendNEC(0x00, 0x1C, noRepeats);
  else if (vry >= 700)
    irsend.sendNEC(0x00, 0x1D, noRepeats);

  
  delay(100);
}