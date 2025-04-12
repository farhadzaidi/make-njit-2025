#include <WiFi.h>
#include "esp_http_server.h"
#include "esp_timer.h"

#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"
#include "camera_config.h"
#include "handlers.h"

#define WIFI_SSID "MakeNJIT"
#define WIFI_PASSWORD "makenjit25"

// Start the web server and register endpoints
static esp_err_t startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server = NULL;
  
  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_uri_t stream_uri = {
      .uri       = "/stream",
      .method    = HTTP_GET,
      .handler   = stream_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &stream_uri);

    return ESP_OK;
  }

  return ESP_FAIL;
}

void setup() {
  Serial.begin(115200);
  while(!Serial);

  // Initialize the camera
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x\n", err);
    return;
  }
  Serial.println("Camera initialized successfully\n");

  // Connect to Wi-Fi
  Serial.printf("Connecting to WiFi: %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected successfully\n");

  // Start the streaming server
  err = startCameraServer();
  if (err != ESP_OK) {
    Serial.printf("Camera server initialization failed with error 0x%x\n", err);
    return;
  }
  Serial.printf("Camera server running on %s\n", WiFi.localIP().toString().c_str());
}

void loop() {
  // The stream is handled by the web server; no code is needed here.
}
