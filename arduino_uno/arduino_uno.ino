#include <DHT.h>
#include <SoftwareSerial.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define RAIN_PIN A0

DHT dht(DHT_PIN, DHT_TYPE);
SoftwareSerial espSerial(10, 11); // RX=10, TX=11 → connects to ESP D6/D7

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);
  dht.begin();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  int   rain = analogRead(RAIN_PIN);
  int   rainPct = map(rain, 0, 1023, 100, 0);

  if (isnan(temp) || isnan(humi)) {
    Serial.println("DHT read failed");
  } else {
    String msg = String(temp, 1) + "," + String(humi, 1) + "," + String(rainPct);
    espSerial.println(msg);
    Serial.println("Sent: " + msg);
  }

  delay(5000);
}