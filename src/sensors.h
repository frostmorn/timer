#pragma once
#include <Arduino.h>
#include <Adafruit_BMP280.h>
#include "config.h"
#define BMP_SENSOR_ADDRESS 0x76
#define SENSOR_TASK_STACK 4096
#define SLEEP_TIME 500 // time between readings

class Sensors{
public:
    Sensors();
    void begin();
    float getTemperature(){ return bmpTemperature;}
    float getPressurePa(){return pressurePa;}
    float getPressureMM(){ return pressureMM;}
private:
    Adafruit_BMP280 bmp; // BOSCH sensor 
    bool bmpStatus;
    void loopTask();
    long long lastDHTread=0;
    float bmpTemperature = 0.0; // Temperature from BOSCH sensor
    float pressurePa = 0.0; // Pressure in Pa
    float pressureMM = 0.0; // Pressure in mm
};
extern Sensors sensors;