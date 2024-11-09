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
#include "buzzer.h"
#include "ui/timesetup.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
#include "esp_log.h"

bool backlight = false;
void toggleBacklight()
{
  backlight = !backlight;
  lcd.setBacklight(backlight);
}
void setup()
{
  Serial.begin(115200);
  Serial.println("Testing Serial output...");
  timer.begin();
  buzzer.begin();
  network.begin();
  lcd.init();
  toggleBacklight();
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
      lcd.print(sensors.getTemperature());
      lcd.print("\1C ");
      lcd.print(sensors.getPressureMM());
      lcd.print("mm");
      lcd.setCursor(0, 1);
     lcd.printf("Humidity = %.2f", sensors.getRelativeHumidity());
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
  menu.addItem("Wi-Fi");
  menu.addItem("B.light On/Off");
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
      network.WiFiMenu();
      break;
    case 3:
      toggleBacklight();
      break;
    default:
      break;
    }
  }
  

}
void mainTask()
{
  while(1){
    mainMenu();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
TaskHandle_t mainTaskH = NULL;

void loop()
{
  // RTOS main menu handler
  if (!mainTaskH){
    xTaskCreate([](void* arg) { mainTask(); }, "mainTask", 4096, NULL, 1, &mainTaskH);
  }
  eTaskState taskState = eTaskGetState(mainTaskH);
  if (taskState == eDeleted) {
    mainTaskH = NULL;
  }

  vTaskDelay(pdMS_TO_TICKS(100));
}
