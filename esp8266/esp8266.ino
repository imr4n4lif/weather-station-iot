#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <SoftwareSerial.h>

// ── CONFIG ───────────────────────────────────────────
const char DEVICE_ID[] = "**";
const char SECRET_KEY[] = "**";
const char SSID[] = "WeatherStation";
const char PASS[] = "11112222";
// ─────────────────────────────────────────────────────

SoftwareSerial unoSerial(D6, D7);  // RX=D6, TX=D7  ← from Uno

float temperature;
float humidity;
int rain;
String incoming = "";

void initProperties() {
  ArduinoCloud.setBoardId(DEVICE_ID);
  ArduinoCloud.setSecretDeviceKey(SECRET_KEY);
  ArduinoCloud.addProperty(temperature, Permission::Read).publishOnChange(0.1);
  ArduinoCloud.addProperty(humidity, Permission::Read).publishOnChange(0.1);
  ArduinoCloud.addProperty(rain, Permission::Read).publishOnChange(1);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void setup() {
  Serial.begin(9600);     // debug via USB Serial Monitor
  unoSerial.begin(9600);  // data from Uno
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  Serial.println("Connecting to Arduino Cloud...");
}

void loop() {
  ArduinoCloud.update();

  while (unoSerial.available()) {
    char c = unoSerial.read();
    if (c == '\n') {
      incoming.trim();
      if (incoming.length() > 0 && incoming.length() < 30) {
        Serial.println("Received from Uno: " + incoming);
        parseAndSend(incoming);
      }
      incoming = "";
    } else {
      incoming += c;
      if (incoming.length() > 40) incoming = "";
    }
  }
}

void parseAndSend(String data) {
  int c1 = data.indexOf(',');
  int c2 = data.lastIndexOf(',');
  if (c1 == -1 || c2 == -1 || c1 == c2) {
    Serial.println("Malformed data, skipping.");
    return;
  }
  temperature = data.substring(0, c1).toFloat();
  humidity = data.substring(c1 + 1, c2).toFloat();
  rain = data.substring(c2 + 1).toInt();
  Serial.println("Sent → Temp: " + String(temperature) + " | Humi: " + String(humidity) + " | Rain: " + String(rain));
}