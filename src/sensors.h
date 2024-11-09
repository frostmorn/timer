#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include "config.h"
#define BMP_SENSOR_ADDRESS 0x76
#define SENSOR_TASK_STACK 4096
#define SLEEP_TIME 3000 // time between readings

class Sensors{
public:
    Sensors();
    void begin();
    float getTemperature(){ return bmpTemperature;}
    float getPressurePa(){return pressurePa;}
    float getPressureMM(){ return pressureMM;}
    float getRelativeHumidity(){ return relativeHumidity;}
private:
    Adafruit_BMP280 bmp; // BOSCH sensor 
    DHT dht{DHT_PIN, DHT_TYPE}; // DHT sensor
    bool bmpStatus;
    void loopTask();
    long long lastDHTread=0;
    float bmpTemperature = 0.0; // Temperature from BOSCH sensor
    float pressurePa = 0.0; // Pressure in Pa
    float pressureMM = 0.0; // Pressure in mm
    float relativeHumidity = 0.0; // humidity in %
};
extern Sensors sensors;