#include "menu.h"
Menu::Menu(LiquidCrystal_I2C *display)
{
    lcd = display;
    redrawNeeded = true;
    finished = false;
    selected = 0;
    controller.setAutoRepeat(Button::TLEFT,1,300);
    controller.setAutoRepeat(Button::TRIGHT,1,300);
    addActicationButton(TOK);
    addActicationButton(TCANCEL);
}
void Menu::update()
{

    State state = controller.getState();
    for (Button activationButton : activationButtons) {
        _StateButtons& buttonsArray = *reinterpret_cast<_StateButtons*>(&state);
        if (buttonsArray[activationButton].justPressed) {
            button = activationButton;
            finished = true;
        }
    }
    if(state.left.justPressed){
        redrawNeeded = true;
        if (selected == 0)
        {
            selected = items.size() - 1;
        }
        else
            selected --;
    } else if (state.right.justPressed){
      redrawNeeded = true;
      selected++;
      if (selected >= items.size())
      selected = 0;
    }
    if (!redrawNeeded)
        return;
    lcd->clear();
    for (size_t i = selected; i-selected < MCOUNT_ROWS; i++)
    {
        lcd->setCursor(0, i-selected);
        char buffer[50];
        if (i < items.size())
        {
            if (selected == i){
                sprintf(buffer, "> %s", items[i].name.c_str());
                
            }
            else{
                sprintf(buffer, "%d %s", i+1, items[i].name.c_str());
            }
            lcd->print(buffer);
        }
        else
        {
            lcd->print("               ");
        }
    }
    redrawNeeded = false;
}