#include <IRremote.h>
#define IR_RECEIVE_PIN 2
// IRrecv irrecv(IR_RECEIVE_PIN);

void setup()
{
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN); // Start the receiver
  // irrecv.enableIRIn();
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
}

void loop() {
  if (IrReceiver.decode()) {
      Serial.print("Raw data: ");
      Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
      Serial.print("Command: ");
      Serial.println(IrReceiver.decodedIRData.command, HEX);
      IrReceiver.resume(); // Enable receiving of the next value
  }
/*
  if (irrecv.decode()) {
    command = irrecv.decodedIRData.command;
    Serial.print("Received command: ");
    Serial.println(command, HEX);
    if (command == 0x1A)
      degree = 45;
    else if (command == 0x1B)
      degree = -90;
    irrecv.resume();
  }
*/
}
