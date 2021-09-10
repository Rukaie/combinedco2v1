#include <M5Stack.h>
#include <Wire.h>
#include "Adafruit_BME680.h"
#include "Ambient.h"
#include <MHZ19.h>
//ライブラリのインポート

#define PERIOD 60
#define RX 16
#define TX 17
#define INTERVAL 6

int count = 0;
int co2 = 0;
//グローバル変数の定義

Adafruit_BME680 bme680; // I2C
MHZ19 myMHZ19;

HardwareSerial  mySerial(1);
//これいるのか不明

void setup() {
  M5.begin();
  Wire.begin(); // I2Cの初期化
  M5.Power.begin();
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("start");

  mySerial.begin(115200, SERIAL_8N1, RX, TX);
  delay(100);
  myMHZ19.begin(mySerial);
  myMHZ19.autoCalibration(false);

  if (!bme680.begin()) {
    Serial.println("Could not find a valid BME680 sensor, check wiring!");
    M5.Lcd.printf("Could not find a valid BME680 sensor, check wiring!");
    while (1) {
      delay(0);
    }
  }
  // Set up oversampling and filter initialization
  bme680.setTemperatureOversampling(BME680_OS_8X);
  bme680.setHumidityOversampling(BME680_OS_2X);
  bme680.setPressureOversampling(BME680_OS_4X);
  bme680.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme680.setGasHeater(320, 150); // 320*C for 150 ms
}


void loop()
{
  if ((count % INTERVAL) == 0 ) {
    co2 = myMHZ19.getCO2();
    displayCo2(co2);
    displayTempHumid();

  } else {
    M5.Lcd.print(".");
  }
  count++;
  delay(1000);
}


void displayCo2(int co2) {
  M5.Lcd.clear();
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("CO2 ppm " + co2);
  //  M5.Lcd.print(co2);
}

void displayTempHumid() {
  float temp, humid, pressure, gas;
  temp = (float)bme680.temperature;//カ氏
  humid = (float)bme680.humidity;
  pressure = (float)bme680.pressure / 100.0;
  gas = (float)bme680.gas_resistance / 1000.0;

  M5.Lcd.println("temp: %2.2f", temp);
  M5.Lcd.println(" humid: %0.2f", humid);
  M5.Lcd.println(" pressure: %f", pressure);
  M5.Lcd.println(" gas: %f\r\n", gas);
}
