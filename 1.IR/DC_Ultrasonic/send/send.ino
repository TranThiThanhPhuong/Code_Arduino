#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

// sensor
const int trigger_pin = 9;
const int pw_pin = 8;
const int trigger_delay = 10;
long duration;
float cm;
//

void setup() {
  Serial.begin(115200);
  pinMode(trigger_pin, OUTPUT);
  pinMode(pw_pin, INPUT);
}
void loop() {
  digitalWrite(trigger_pin, LOW);
  delayMicroseconds(10);
  digitalWrite(trigger_pin, HIGH);
  delayMicroseconds(trigger_delay);
  digitalWrite(trigger_pin, LOW);
  duration = pulseIn(pw_pin, HIGH);
  cm = duration * 0.017;
  Serial.print(cm);
  Serial.println(" cm");

  if (cm <= 12.5)
    irsend.sendNEC(0x00, 0x1A, noRepeats); 
  else
    irsend.sendNEC(0x00, 0x1B, noRepeats); 
    
  delay(100);
}