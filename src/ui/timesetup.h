#pragma once
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "controller.h"
#include "buzzer.h"
#include "utils.h"
#include "timer.h"
#define TIME_FMT_STR "    %02d:%02d:%02d    "
#define HH_POS_STR "    hh"
#define MM_POS_STR "       mm"
#define SS_POS_STR "          ss"

typedef enum{
    pNONE=0,
    pHH,
    pMM,
    pSS,
}CursorPos;

class TimeSetup{
public:
    TimeSetup(LiquidCrystal_I2C *display);
    void update();
    void updateFromTimer();
    bool isFinished(){return finished;}
    bool isConfirmed(){return confirm;}
    void countdown();
    unsigned long getTimeSet(){
        return Utils::timeToMillis(hh, mm, ss);
    }
private:
    void redraw();
    LiquidCrystal_I2C *lcd;
    uint32_t startTimer = 0;
    CursorPos cursorPos = pHH;
    uint8_t hh = 0;
    uint8_t mm = 0;
    uint8_t ss = 0;
    bool confirm = false;
    bool redrawNeeded = true;
    bool finished = false;
};