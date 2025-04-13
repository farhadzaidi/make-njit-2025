#include <WiFi.h>
#include "esp_http_server.h"
#include "esp_camera.h"

#include "camera_config.h"
#include "handlers.h"
#include "wifi_info.h"
#include "motor.h"

// Start the web server and register endpoints
static esp_err_t start_server() {
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

    httpd_uri_t move_uri = {
      .uri       = "/move",
      .method    = HTTP_GET,
      .handler   = move_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &move_uri);

    httpd_uri_t test_uri = {
      .uri       = "/test",
      .method    = HTTP_GET,
      .handler   = test_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &test_uri);

    httpd_uri_t options_uri = {
      .uri       = "/*",
      .method    = HTTP_OPTIONS,
      .handler   = options_handler,
      .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &options_uri);

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
  err = start_server();
  if (err != ESP_OK) {
    Serial.printf("Camera server initialization failed with error 0x%x\n", err);
    return;
  }
  Serial.printf("Camera server running on %s\n", WiFi.localIP().toString().c_str());

  // Initialize motors
  setup_motors();
}

void loop() {
  // Do nothing
}
