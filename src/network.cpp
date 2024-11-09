#include "network.h"
#include "display.h"
#include "network.h"
#include "ui/menu.h"
#include "sensors.h"
#include "ui/timesetup.h"
#include "utils.h"
#include <esp_wifi.h>
#include <esp_event.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_netif.h>

char buffer[1024];

static const char* html_template = R"(
<!DOCTYPE html>
<html>
<head><title>Timer Status</title></head>
<body>
    <h1>Timer Status</h1>
    <div>Uptime: %s</div>
    <div>Temperature: <p class='temp'>%.2f</p> C</div>
    <div>Pressure: <p class='pressure-pa'>%.2f</p> Pa</div>
    <div>Pressure: <p class='pressure-mm'>%.2f</p> mm</div>
    <div>Timer state: <p class='timer-state'>%d</p></div>
    <div>Time left to tick: <p class='timer-end'>%s</p></div>
    <div>Last reset reason: </p class='reset-reason'>%d</p>
    <div>Free heap: <p class='heap'>%d</p> bytes</div>
</body>
</html>
)";

static const char *TAG = "Network";
String Network::wifiStatusToString(wl_status_t status) {
  switch (status) {
    case WL_CONNECTED:
      return "Connected";
    case WL_NO_SSID_AVAIL:
      return "NO SSID";
    case WL_CONNECT_FAILED:
      return "CONNECT FAILED";
    case WL_IDLE_STATUS:
      return "Idle";
    case WL_DISCONNECTED:
      return "Disconnected";
    default:
      return "Unknown";
  }
}

// Constructor
Network::Network(){}

// Wi-Fi initialization method
void Network::begin() {
    WiFi.begin();
    startServer();
}

// HTTP GET request handler for "/"
esp_err_t Network::helloGetHandler(httpd_req_t *req) {

    sprintf(buffer, html_template,  Utils::millisToTimeFormat(millis()),
          sensors.getTemperature(), 
           sensors.getPressurePa(),
            sensors.getPressureMM(),
            !timer.isFinished(),
            timer.getTimeLeftStr(), esp_reset_reason(), esp_get_free_heap_size());
    return httpd_resp_send(req, buffer, strlen(buffer));
}

// Start the web server and register URI handlers
httpd_handle_t Network::startWebServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    config.task_priority = tskIDLE_PRIORITY+1;
    config.send_wait_timeout = 1;
    config.recv_wait_timeout = 1;
    config.lru_purge_enable = 1;
    

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t helloUri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = helloGetHandler,
            .user_ctx = NULL
        };
        httpd_register_uri_handler(server, &helloUri);
        ESP_LOGI(TAG, "Web server started");
    }
    return server;
}

// Start the server from the Network class
void Network::startServer() {
    if (server == NULL) {
        server = startWebServer();
    }
}

void Network::WiFiMenu()
{
    Menu menu(&lcd);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(false);
    WiFi.disconnect();
    lcd.clear();
    lcd.print("Scaning networks...");
    int n = WiFi.scanNetworks();
    if (n == 0)
    {
        lcd.clear();
        lcd.print("No networks found!");
        vTaskDelay(pdMS_TO_TICKS(1000));
        return;
    }
    else
    {
        Menu menu(&lcd);
        for (size_t i = 0; i < n; i++)
        {
            menu.addItem(WiFi.SSID(i));
        }

        while (!menu.isFinished())
        {
            // delay(500);
            vTaskDelay(pdMS_TO_TICKS(5));
            menu.update();
        }
        if (menu.getButton() == TOK)
        {
            WiFi.begin(WiFi.SSID(menu.getSelected()), "");
            lcd.clear();
            lcd.print("Connecting");
            auto startConnect = millis();
            
            while (WiFi.status() != WL_CONNECTED)
            {
                vTaskDelay(pdMS_TO_TICKS(100));
                if (millis() - startConnect > WIFI_CONNECT_TIMEOUT)
                {
                    lcd.clear();
                    lcd.print("Timeout reached");
                    lcd.setCursor(0, 1);
                    lcd.print(wifiStatusToString(WiFi.status()));
                    vTaskDelay(pdMS_TO_TICKS(5000));
                    return;
                }
            }
            lcd.clear();
            lcd.print("IP address :");
            lcd.setCursor(0, 1);
            lcd.print(WiFi.localIP());
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
    }
}


Network network;