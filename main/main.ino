// To use serial you MUST have tools -> USB cdc on boot = enabled !!!

// To connect this Iot device to Wifi:
// 1) Power it
// 2) It will produce a wifi hotspot that you connect to and setup wifi
// 3) The device will remain in that wifi until it cant get that wifi. In that case it will make a hotspot again.

#include <DFRobot_ENS160.h>
//#include <SensirionI2CSen5x.h>
#include <Wire.h>
#include <WiFiManager.h>
#include "AdafruitIO_WiFi.h"
#include "config.h"

#define uS_TO_S_FACTOR 1000000ULL // Conversion factor for micro seconds to seconds
DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);

const bool debug = false;
bool led_status = 1;

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "", "");  // Dont give wifi name and password. Do this with wifimanager
AdafruitIO_Feed *co2 = io.feed("co2");
AdafruitIO_Feed *voc = io.feed("voc");

void setup_ens160(float temperature, float humidity){
  // Assumes you have made Serial.begin before this func

  // Init ENS160
  while( NO_ERR != ENS160.begin() ){
    if(debug){
      Serial.println("Communication with device failed, please check connection");
      delay(1000);
    }
  }
  //ENS160.setPWRMode(ENS160_STANDARD_MODE);
  // We are returning the ENS160 from deep sleep. Give the ens a little time to warm up
  //delay(1000);
  /**
   * Set power mode
   * mode Configurable power mode:
   *   ENS160_SLEEP_MODE: DEEP SLEEP mode (low power standby)
   *   ENS160_IDLE_MODE: IDLE mode (low-power)
   *   ENS160_STANDARD_MODE: STANDARD Gas Sensing Modes
   */

  /**
   * Users write ambient temperature and relative humidity into ENS160 for calibration and compensation of the measured gas data.
   * ambientTemp Compensate the current ambient temperature, float type, unit: C
   * relativeHumidity Compensate the current ambient temperature, float type, unit: %rH
   */
  ENS160.setTempAndHum(temperature, humidity);
}

void setup() {
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  // wm.resetSettings();
  wm.autoConnect("KallesIotAirQualitySensor");

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  setup_ens160(/*temperature=*/25.0, /*humidity=*/50.0);  // TODO: change these to be the measurements from P2.5 sensor

  io.connect();
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    if(debug){
      Serial.print(".");
      delay(500);
    }
  }
  // we are connected
  if(debug){
    Serial.println();
    Serial.println(io.statusText());
  }

  esp_sleep_enable_timer_wakeup(10 * uS_TO_S_FACTOR);
}

void loop() {
  uint8_t Status = 7;
  uint8_t  AQI   = 123;

/*
  while(Status != 0){  // Wait until sensor has warmed up. This means 3 minutes from startup. AND to leave status 2 you have to run the device for 24h straight at least once.
    Status = ENS160.getENS160Status(); // 0-Normal, 1-Warm-Up, 2-initial
    if(debug){
      Serial.println("waiting for sensor");
      Serial.println(Status);
    }
  }
*/

  uint16_t TVOC  = ENS160.getTVOC();         // Total volatile organic compounds
  uint16_t ECO2  = ENS160.getECO2();         // co2 equivalent

  // TODO: I did not get sensor to wake up fast enough. It always showed 400 ppm. TODO: Make sensor power efficient.
  //ENS160.setPWRMode(ENS160_IDLE_MODE);      // We will return external system from sleep upon entrance from sleep
  co2->save(ECO2);
  voc->save(TVOC);
  io.run();

  if(debug){
    led_status = !led_status;
    digitalWrite(LED_BUILTIN, led_status);

    Serial.print("Sensor operating status : ");
    Serial.println(Status);
    Serial.print("Air quality index : ");
    AQI = ENS160.getAQI();          // air quality index. 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
    Serial.println(AQI);
    Serial.print("Concentration of total volatile organic compounds : ");
    Serial.print(TVOC);
    Serial.println(" ppb");
    Serial.print("Carbon dioxide equivalent concentration : ");
    Serial.print(ECO2);
    Serial.println(" ppm");
    Serial.println();
  }

  esp_deep_sleep_start();
}