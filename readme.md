# Weather Station

Reads temperature, humidity, and rain level and sends the data live to the Arduino IoT Cloud.

---

## How it works

1. **Power** — A 8.4V Li-ion battery pack (two 3.7V cells in series) feeds a buck converter that steps it down to 5V. Both boards run off this 5V output.

2. **Sensing** — The Arduino Uno reads the DHT11 sensor (temperature + humidity) and the rain sensor every 5 seconds.

3. **Sending** — The Arduino formats the readings as a simple string like `25.3,60.1,12` and sends it to the ESP8266 over a serial wire.

4. **Voltage divider** — The Arduino talks at 5V but the ESP8266 only tolerates 3.3V on its RX pin. A 1kΩ + 2kΩ resistor divider sits between them to drop the signal safely.

5. **Cloud** — The ESP8266 parses the string, extracts the three values, and pushes them to the Arduino IoT Cloud over Wi-Fi. Values only upload when they change.

---

## Wiring diagram

![Wiring Diagram](diagram.png)

---

## Notes

- Both boards must share a common ground for serial communication to work.
- Buck converter should be rated at least 1A output.