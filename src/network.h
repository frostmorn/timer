#ifndef NETWORK_H
#define NETWORK_H
#include <WiFi.h>
#include <esp_http_server.h>
#define WIFI_CONNECT_TIMEOUT 5000

class Network {
public:
    // Constructor to initialize with Wi-Fi SSID and Password
    Network();
    // Starts the HTTP server
    void startServer();
    void begin();
    void WiFiMenu();
private:
    // Starts the HTTP server and registers URI handlers
    httpd_handle_t startWebServer();

    // HTTP GET request handler for the root URI "/"
    static esp_err_t helloGetHandler(httpd_req_t *req);
    httpd_handle_t server;        // HTTP server handle
};

extern Network network;
#endif // NETWORK_H
