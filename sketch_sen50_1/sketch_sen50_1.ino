#include <SensirionI2CSen5x.h>
#include <Wire.h>

SensirionI2CSen5x sen5x;

void setup() {
  Serial.begin(115200);
  delay(3000);
  while (!Serial);

  Wire.begin(19, 20);  // SDA, SCL
  Serial.println("Setup");

  uint16_t error;
  char errorMessage[256];

  sen5x.begin(Wire);

  error = sen5x.deviceReset();
  if (error) {
    Serial.print("Device reset failed: ");
    Serial.println(error);
    return;
  }
  else{
    Serial.println("No error 1");
  }

  delay(200);  // let it stabilize

  error = sen5x.startMeasurement();
  if (error) {
    Serial.print("Start measurement failed: ");
    Serial.println(error);
    return;
  }
  else{
    Serial.println("No error 2");
  }
}

void loop() {
  float pm1, pm2p5, pm4, pm10, humidity, temp, voc, nox;
  uint16_t error = sen5x.readMeasuredValues(pm1, pm2p5, pm4, pm10, humidity, temp, voc, nox);
  if (error){
    Serial.print("Read error: ");
    Serial.println(error);
  } else {
    Serial.print("PM2.5: "); Serial.println(pm2p5);
  }
  delay(1000);
}
