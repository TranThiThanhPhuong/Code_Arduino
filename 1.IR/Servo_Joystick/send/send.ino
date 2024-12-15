#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

// sensor
int vrx, vry;
//

void setup() {
  Serial.begin(9600);
}


void loop() {
  vrx = analogRead(A0);
  vry = analogRead(A1);
  Serial.print("x: ");
  Serial.print(vrx);
  Serial.print(", y: ");
  Serial.print(vry);
  Serial.println();
  // Gạt xuống
  if (vrx <= 200)
    irsend.sendNEC(0x00, 0x1A, noRepeats);
  // Gạt lên  
  else if (vrx >= 800)
    irsend.sendNEC(0x00, 0x1B, noRepeats);
  // Gạt trái
  if (vry <= 200)
    irsend.sendNEC(0x00, 0x1C, noRepeats);
  if (vry >= 800)
    irsend.sendNEC(0x00, 0x1D, noRepeats);    
  delay(100);
}