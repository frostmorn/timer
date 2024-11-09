#pragma once
#include <Arduino.h>
namespace Utils
{
    String millisToTimeFormat(unsigned long millis);
    unsigned long timeToMillis(int hours, int minutes, int seconds);
}