#include <DHT.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 2
#define DHT_TYPE DHT11
#define RAIN_PIN A0

DHT dht(DHT_PIN, DHT_TYPE);
SoftwareSerial espSerial(10, 11);

// LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Variables
float temp = 0;
float humi = 0;
int rainPct = 0;

// Timers
unsigned long lastLCD = 0;
unsigned long lastSend = 0;

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Weather System");
  delay(2000);
  lcd.clear();
}

void loop() {

  // Read sensors
  float t = dht.readTemperature() - 1.5; // calibration
  float h = dht.readHumidity();
  int rain = analogRead(RAIN_PIN);
  int r = map(rain, 0, 1023, 100, 0);

  if (!isnan(t) && !isnan(h)) {
    temp = t;
    humi = h;
    rainPct = r;
  }

  // LCD update every 0.5 sec
  if (millis() - lastLCD >= 500) {
    lastLCD = millis();

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(temp, 1);
    lcd.print((char)223);
    lcd.print("C ");

    lcd.print("H:");
    lcd.print(humi, 0);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Rain:");
    lcd.print(rainPct);
    lcd.print("%");
  }

  // Send to ESP/Serial every 5 sec
  if (millis() - lastSend >= 5000) {
    lastSend = millis();

    String msg = String(temp, 1) + "," +
                 String(humi, 1) + "," +
                 String(rainPct);

    espSerial.println(msg);
    Serial.println("Sent: " + msg);
  }
}