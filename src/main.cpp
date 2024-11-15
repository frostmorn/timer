#include <Arduino.h>
#include "sensors.h"
#include "config.h"
#include "ui/menu.h"
#include "display.h"
#include "controller.h"
#include <WiFi.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "sensors.h"
#include "network.h"
#include "ui/timesetup.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "esp_log.h"
#include "settings.h"

void setup()
{
  Serial.begin(115200);
  Serial.println("Testing Serial output...");
  settings.begin();
  timer.begin();
  buzzer.begin();
  network.begin();
  lcd.init();
  controller.begin();
  sensors.begin();
  byte degreeSymbol[8] = DEGREE_SYMBOL;
  lcd.createChar(1, degreeSymbol);
}
void readTempPressure()
{
  auto i = 0;
  while (1)
  {
    if (i == 200 * 5)
      i = 0;
    if (i == 0)
    {
      lcd.clear();
      lcd.printf("%.2f\1C %.2fmm", sensors.getTemperature(), sensors.getPressureMM());
      lcd.setCursor(0, 1);
     lcd.printf("%.2f Pa", sensors.getPressurePa());
    }
    i++;
    vTaskDelay(pdMS_TO_TICKS(5));
    auto state = controller.getState();
    if (state.cancel.justPressed)
      return;
  }
}
void Timer()
{

  TimeSetup ts(&lcd);
  if (!timer.isFinished())
  {
    ts.updateFromTimer();
    return;
  }
  else
  {
    while (!ts.isFinished())
    {
      ts.update();
      vTaskDelay(pdMS_TO_TICKS(5));
    }
    if (ts.isConfirmed()){
      timer.setTimer(ts.getTimeSet());
      ts.updateFromTimer();
    }
  }
}
void mainMenu()
{
  Menu menu(&lcd);
  menu.addItem("Timer");
  menu.addItem("Sensors");
  menu.addItem("Settings");
  while (!menu.isFinished())
  {
    // delay(500);
    vTaskDelay(pdMS_TO_TICKS(5));
    menu.update();
  }
  if (menu.getButton() == Button::TOK)
  {
    auto selected = menu.getSelected();
    switch (selected)
    {
    case 0:
      Timer();
      break;
    case 1:
      readTempPressure();
      break;
    case 2:
      settings.mainMenu();
      break;
    default:
      break;
    }
  }
  

}

void loop()
{
  while(1){
    mainMenu();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
