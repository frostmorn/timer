#include "sensors.h"
Sensors::Sensors(){
}
void Sensors::begin(){
    // initialize sensors
    dht.begin();
    bmpStatus = bmp.begin(BMP_SENSOR_ADDRESS);
    // Configure sensors
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */   
    // Run loopTask performing sensors reading
    xTaskCreate([](void* arg) { static_cast<Sensors*>(arg)->loopTask();}, "Sensors Reading",SENSOR_TASK_STACK, this, 1, NULL);
}
void Sensors::loopTask(){
    while (1){
        // Read sensors data
            // BMP:
        bmpTemperature = bmp.readTemperature();
        pressurePa = bmp.readPressure();
        pressureMM = pressurePa / 133.322; // some constant value I forgot name
        // Ensure it doesn't read too much
        if (millis() - lastDHTread > 3000){
            auto humidityBuffer = dht.readHumidity(true);
            relativeHumidity = isnan(humidityBuffer)? relativeHumidity:humidityBuffer;
            lastDHTread = millis();
        }

        // Do pause till next sensors check
        vTaskDelay(pdMS_TO_TICKS(SLEEP_TIME));
    }

}

Sensors sensors;