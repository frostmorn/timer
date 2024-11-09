#include "timer.h"
void Timer::begin(){
     // Create RTOS task for handling button presses
    xTaskCreate([](void* arg) { static_cast<Timer*>(arg)->timerLoop(); }, "TimerTask", 2048, this, 1, NULL);
}

bool Timer::isFinished()
{
    return finished;
}
unsigned long Timer::getTimeLeft()
{
    return waitTime-(millis() - start);
}
String Timer::getTimeLeftStr()
{
    return Utils::millisToTimeFormat(getTimeLeft());
}
void Timer::setTimer(unsigned long time_wait)
{
    start = millis();
    waitTime = time_wait;
    finished = false;
}
void Timer::clearTimer(){
    finished = true;
}

void Timer::timerLoop(){
    while (1){
        if (!finished){
            while ((millis() - start) < waitTime) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }
            buzzer.doMarioSong();
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
Timer timer;