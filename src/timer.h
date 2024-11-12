#pragma once
#include <Arduino.h>
#include "utils.h"
#include "buzzer/buzzer.h"
class Timer{
public:
    Timer(){}
    void begin();
    bool isFinished();
    unsigned long getTimeLeft();
    String getTimeLeftStr();
    void setTimer(unsigned long time_wait);
    void clearTimer();
private:
    void timerLoop();
    unsigned long waitTime;
    unsigned long start; // When new timer is set, this value set to millis()
    bool finished=true;
};

extern Timer timer;