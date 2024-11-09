#pragma once
#include <Arduino.h>
#include <vector>
#include <LiquidCrystal_I2C.h>
#include "controller.h"
#define MCOUNT_ROWS 2
typedef struct {
    String name;

}
MenuItem;
class Menu{
public:
    Menu(LiquidCrystal_I2C *display);
    void update();
    void addItem(String itemName){
        items.push_back({itemName});
    }
    void addActicationButton(Button activationButton){
        activationButtons.push_back(activationButton);
    }   
    bool isFinished(){return finished;}
    size_t getSelected(){
        return selected;
    }
    Button getButton(){
        return button;
    }
private:
    LiquidCrystal_I2C * lcd;
    size_t selected;
    bool redrawNeeded;
    bool finished ;
    std::vector<Button> activationButtons;
    Button button;
    std::vector<MenuItem> items;
};
