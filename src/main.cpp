/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
  See the LICENSE file for details.
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
 
#define SEALEVELPRESSURE_HPA (1011)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

void setup() {
    Serial.begin(115200);
    while(!Serial);   
    Serial.println(F("BME280 test"));

    unsigned status;
    
    status = bme.begin(0x76);  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        while (1) delay(10);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;
    Serial.println();
}


void loop() { 
    delay(5000);
    Serial.print("Temperatura = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Ciśnienie = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Przybliżona wysokość = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA) );
    Serial.println(" m");

    Serial.print("Wilgotność = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}