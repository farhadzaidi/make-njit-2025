#include <WiFi.h>
#include "esp_camera.h"
#include "esp_http_server.h"
#include "esp_timer.h"
#include "wifi_info.h" 

#define CAMERA_CLOCK_SPEED 20000000 // 20 MHz
#define JPEG_QUALITY 10 // 0-63, lower = quality
#define FRAME_BUFFER_COUNT 2

#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"

// Configure camera
camera_config_t camera_config = {
  .pin_pwdn       = PWDN_GPIO_NUM,
  .pin_reset      = RESET_GPIO_NUM,
  .pin_xclk       = XCLK_GPIO_NUM,
  .pin_sccb_sda   = SIOD_GPIO_NUM,
  .pin_sccb_scl   = SIOC_GPIO_NUM,
  .pin_d7         = Y9_GPIO_NUM,
  .pin_d6         = Y8_GPIO_NUM,
  .pin_d5         = Y7_GPIO_NUM,
  .pin_d4         = Y6_GPIO_NUM,
  .pin_d3         = Y5_GPIO_NUM,
  .pin_d2         = Y4_GPIO_NUM,
  .pin_d1         = Y3_GPIO_NUM,
  .pin_d0         = Y2_GPIO_NUM,
  .pin_vsync      = VSYNC_GPIO_NUM,
  .pin_href       = HREF_GPIO_NUM,
  .pin_pclk       = PCLK_GPIO_NUM,
  .xclk_freq_hz   = CAMERA_CLOCK_SPEED,
  .ledc_timer     = LEDC_TIMER_0,
  .ledc_channel   = LEDC_CHANNEL_0,
  .pixel_format   = PIXFORMAT_JPEG,    
  .frame_size     = FRAMESIZE_SVGA,    
  .jpeg_quality   = JPEG_QUALITY,                
  .fb_count       = FRAME_BUFFER_COUNT,
  .fb_location    = CAMERA_FB_IN_PSRAM,
  .grab_mode      = CAMERA_GRAB_WHEN_EMPTY
};

// Stream response definitions
static const char *STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=frame";
static const char *STREAM_BOUNDARY = "\r\n--frame\r\n";
static const char *STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

// Use this endpoint to stream frames using MJPEG
static esp_err_t stream_handler(httpd_req_t *req) {
  // Set HTTP response header
  httpd_resp_set_type(req, STREAM_CONTENT_TYPE);

  while (true) {
    // Capture frame
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }

    // Prepare the header for this frame
    char header_buf[64];
    int header_len = snprintf(header_buf, sizeof(header_buf), STREAM_PART, fb->len);
    
    // Send the boundary marker to separate frames
    esp_err_t res = httpd_resp_send_chunk(req, STREAM_BOUNDARY, strlen(STREAM_BOUNDARY));
    if (res != ESP_OK) {
      esp_camera_fb_return(fb);
      return res;
    }

    // Send the header containing frame length
    res = httpd_resp_send_chunk(req, header_buf, header_len);
    if (res != ESP_OK) {
      esp_camera_fb_return(fb);
      return res;
    }

    // Send the actual JPEG data
    res = httpd_resp_send_chunk(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
    if (res != ESP_OK) return res;
  }  
}

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
