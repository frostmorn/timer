#include "timesetup.h"

TimeSetup::TimeSetup(LiquidCrystal_I2C *display){
    lcd = display;

    controller.setAutoRepeat(Button::TLEFT,1,300);
    controller.setAutoRepeat(Button::TRIGHT,1,300);
    controller.setAutoRepeat(Button::TOK,2,500);
    controller.setAutoRepeat(Button::TCANCEL,2,500);
}
void TimeSetup::update(){
    
    auto state = controller.getState();
    // if L and R pressed time seems to be set
    if (state.left.pressed && state.right.pressed){
        finished = true;
        confirm = true;
        return;
    } else if (state.ok.pressed && state.cancel.pressed){
        finished = true;
        confirm = false;
    }
    // Iterate between pHH pMM and pSS
    if (state.left.justPressed){
        redrawNeeded = true;
        switch (cursorPos)
        {
        case pHH:
            cursorPos = pSS;
            break;
        case pMM:
            cursorPos = pHH;
            break;
        case pSS:
            cursorPos = pMM;
            break;        
        default:
            cursorPos = pHH;
            break;
        }
    } else if (state.right.justPressed){
        redrawNeeded = true;
        switch (cursorPos)
        {
        case pHH:
            cursorPos = pMM;
            break;
        case pMM:
            cursorPos = pSS;
            break;
        case pSS:
            cursorPos = pHH;
            break;        
        default:
            cursorPos = pHH;
            break;
        }         
    } else if (state.ok.justPressed){
        redrawNeeded = true;
        switch (cursorPos)
        {
        case pHH:
            hh++;
            break;
        case pMM:
            mm++;
            break;
        case pSS:
            ss++;
            break;        
        default:
            break;
        }
    }else if (state.cancel.justPressed){
        redrawNeeded = true;
        switch (cursorPos)
        {
        case pHH:
            if (hh != 0)
                hh--;
            break;
        case pMM:
            if (mm != 0)
                mm--;
            break;
        case pSS:
            if (ss!= 0)
            ss--;
            break;        
        default:
            break;
        }
    }
    // fix overflow problem
    if (ss >= 60){
        mm++;
        ss = 0;
    }
    if (mm >= 60){
        hh++;
        mm = 0;
    }
    if (hh >= 99){
        hh = 0;
    }

    if (!redrawNeeded)
        return;
    redraw();
    redrawNeeded = false;
}
void TimeSetup::updateFromTimer(){
    uint16_t i = 0;
    while (!timer.isFinished()){
        if (i == 0){
            lcd->clear();
            lcd->print("    "+timer.getTimeLeftStr());
        }
        auto state = controller.getState();
        if (state.cancel.justPressed){
            timer.clearTimer();
            return;
        } else if (state.ok.justPressed){
            return;
        }
        i++;
        if (i == 100)
            i = 0;
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void TimeSetup::redraw(){
    lcd->clear();
    lcd->setCursor(0,0);
    lcd->printf(TIME_FMT_STR, hh, mm, ss);
    lcd->setCursor(0,1);
    switch (cursorPos)
        {
        case pHH:
            lcd->printf(HH_POS_STR);
            break;
        case pMM:
            lcd->printf(MM_POS_STR);
            break;
        case pSS:
            lcd->printf(SS_POS_STR);
            break;        
        default:
            //lcd->printf(HH_POS_STR);
            break;
    }
}