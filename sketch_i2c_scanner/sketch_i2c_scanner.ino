#include <Wire.h>

void setup() {
  Wire.begin(19, 20); // your SDA and SCL
  Serial.begin(115200);
  while (!Serial);
  delay(1000);
  Serial.println("I2C scanner running...");

  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      Serial.println(address, HEX);
      nDevices++;
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found.\n");
  else
    Serial.println("Done.\n");
}

void loop() {}
