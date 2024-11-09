#include "utils.h"
namespace Utils
{
    String millisToTimeFormat(unsigned long millis)
    {
        unsigned long seconds = millis / 1000;
        unsigned long minutes = seconds / 60;
        unsigned long hours = minutes / 60;

        seconds = seconds % 60; // Remaining seconds
        minutes = minutes % 60; // Remaining minutes

        String timeString = "";
        if (hours < 10)
            timeString += "0"; // Pad with zero if needed
        timeString += String(hours) + ":";

        if (minutes < 10)
            timeString += "0";
        timeString += String(minutes) + ":";

        if (seconds < 10)
            timeString += "0";
        timeString += String(seconds);

        return timeString;
    }
    unsigned long timeToMillis(int hours, int minutes, int seconds)
    {
        unsigned long millis = 0;
        millis += hours * 3600000UL; // Convert hours to milliseconds
        millis += minutes * 60000UL; // Convert minutes to milliseconds
        millis += seconds * 1000UL;  // Convert seconds to milliseconds
        return millis;
    }
}