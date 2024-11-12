#include "settings.h"
#include "ui/menu.h"
#include "display.h"
#include "network.h"

Settings::Settings(){

}

void Settings::begin(){
    toggleBacklight();
}

void Settings::toggleBacklight()
{
  backlight = !backlight;
  lcd.setBacklight(backlight);
}

void Settings::mainMenu(){
    Menu menu(&lcd);
    menu.addItem("Timer song");
    menu.addItem("Wi-Fi");
    menu.addItem("Toggle BLight");
    while (!menu.isFinished()){
        vTaskDelay(pdMS_TO_TICKS(5));
        menu.update();
    }
    if (menu.getButton() == TOK){
        switch (menu.getSelected())
        {
        case 0:
            songSelectionMenu();
            break;
        case 1:
            network.WiFiMenu();
            break;
        case 2:
            toggleBacklight();
            break;
        }
    }
}
void Settings::songSelectionMenu(){
    Menu menu(&lcd);
    for (auto melody:melodies){
        menu.addItem(melody->name);
    }
    while(!menu.isFinished()){
        // play melody here
        vTaskDelay(pdMS_TO_TICKS(5));
        menu.update();
    }
    if(menu.getButton() == TOK){
        buzzer.setDefaultMelody(melodies[menu.getSelected()]);
    }
}

Settings settings;