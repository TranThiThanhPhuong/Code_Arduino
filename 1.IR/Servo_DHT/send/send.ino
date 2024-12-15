#include <IRremote.h>
#define IR_SEND_PIN 2
IRsend irsend(IR_SEND_PIN);
uint32_t noRepeats = 2;

// sensor
#include <DHT.h>
const int dht11_pin = 8;
const int dht_model = DHT11;
float humidity;
float temperature;
DHT dht11(dht11_pin, dht_model);
//

void setup() {
  Serial.begin(9600);
  dht11.begin();  
}


void loop() {
  humidity = dht11.readHumidity();
  temperature = dht11.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Không có giá trị!");
  } else {
    Serial.print("Độ ẩm: ");
    Serial.print(humidity);

    Serial.print("%; Nhiêt độ: ");
    Serial.print(temperature);
    Serial.println("\u2103");
  }
    if (temperature >= 30 && humidity >= 85)
      irsend.sendNEC(0x00, 0x1A, noRepeats);
    else
      irsend.sendNEC(0x00, 0x1B, noRepeats);
  delay(100);
}