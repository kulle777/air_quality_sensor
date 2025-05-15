// To use serial you MUST have tools -> USB cdc on boot = enabled !!!

#include <DFRobot_ENS160.h>
//#include <SensirionI2CSen5x.h>
#include <Wire.h>
//#include "config.h"

DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

const bool verbous = true;
bool led_status = 1;

void setup_ens160(float temperature, float humidity){
  // Assumes you have made Serial.begin before this func

  // Init ENS160
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(1000);
  }
  /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  /**
   * Users write ambient temperature and relative humidity into ENS160 for calibration and compensation of the measured gas data.
   * ambientTemp Compensate the current ambient temperature, float type, unit: C
   * relativeHumidity Compensate the current ambient temperature, float type, unit: %rH
   */
  ENS160.setTempAndHum(temperature, humidity);
}

void setup() {
  Serial.begin(115200);
  setup_ens160(/*temperature=*/25.0, /*humidity=*/50.0);  // TODO: change these to be the measurements from P2.5 sensor

  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  uint8_t Status = ENS160.getENS160Status(); // 0-Normal, 1-Warm-Up, 2-initial
  uint8_t AQI    = ENS160.getAQI();          // air quality index. 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
  uint16_t TVOC  = ENS160.getTVOC();
  uint16_t ECO2  = ENS160.getECO2();

  if(verbous){
    Serial.print("Sensor operating status : ");
    Serial.println(Status);
    Serial.print("Air quality index : ");
    Serial.println(AQI);
    Serial.print("Concentration of total volatile organic compounds : ");
    Serial.print(TVOC);
    Serial.println(" ppb");
    Serial.print("Carbon dioxide equivalent concentration : ");
    Serial.print(ECO2);
    Serial.println(" ppm");
    Serial.println();
  }

  led_status = !led_status;
  digitalWrite(LED_BUILTIN, led_status);
  delay(1000);
}