#pragma once
#include "ui/menu.h"
#include "buzzer/melodies.h"
#include "buzzer/buzzer.h"
// TODO:
// Buzzer Song selection
// Wifi Scan
// UART Console
// Save settings to NVS
// Auto load settings from NVS
// Translate to json
// Load from json

class Settings{
public:
    Settings();
    void begin();
    void mainMenu();
    void toggleBacklight();
private:
    boolean backlight = false;
    void songSelectionMenu();
    //void LoadFromNVS();
};
extern Settings settings;