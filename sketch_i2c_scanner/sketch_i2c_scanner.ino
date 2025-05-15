
#include <Wire.h>

void setup() {
  Wire.begin(19, 20); // SDA, SCL
  Serial.begin(115200);
  delay(3000);
  Serial.println("Scanning I2C bus...");

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("Found I2C device at 0x");
      Serial.println(address, HEX);
    }
  }
}

void loop() {}