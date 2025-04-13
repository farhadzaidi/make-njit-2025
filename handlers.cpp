#include "Arduino.h"
#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_camera.h"
#include "motor.h"

// Use this endpoint to stream frames using MJPEG
esp_err_t stream_handler(httpd_req_t *req) {
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  // Stream response definitions
  static const char *STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=frame";
  static const char *STREAM_BOUNDARY = "\r\n--frame\r\n";
  static const char *STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

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

esp_err_t move_handler(httpd_req_t *req) {
  Serial.println("Inside move handler");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");

  char query[64];
  char action[32] = {0};
  char pwm_str[16] = {0};
  int pwm_val = 100;

  // Retrieve query string
  if (httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK) {
    Serial.println("Retrieved query string");
    // Retrieve "action" parameter
    if (httpd_query_key_value(query, "action", action, sizeof(action)) != ESP_OK) {
      httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Unable to retrieve 'action' query parameter");
      return ESP_FAIL;
    }
    Serial.printf("Retrieved action parameter: %s\n", action);

    // Retrieve "pwm" parameter; if one isn't provided, fallback to default
    if (httpd_query_key_value(query, "pwm", pwm_str, sizeof(pwm_str)) == ESP_OK) {
      pwm_val = atoi(pwm_str);
    }
  } else {
    Serial.println("Failed to retrieve query parameters");
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Unable to retrieve query string");
    return ESP_FAIL;
  }

  if (strcmp(action, "forward") == 0) set_motors_forward(pwm_val);
  else if (strcmp(action, "reverse") == 0) set_motors_reverse(pwm_val);
  else if (strcmp(action, "clockwise") == 0) set_motors_clockwise(pwm_val);
  else if (strcmp(action, "counter_clockwise") == 0) set_motors_counter_clockwise(pwm_val);
  else if (strcmp(action, "activate_servo") == 0) activate_servo_motor();
  else if (strcmp(action, "deactivate_servo") == 0) deactivate_servo_motor();
  else if (strcmp(action, "stop") == 0) stop_motors();
  else {
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid action");
    return ESP_FAIL;
  }

  Serial.println("performed action");
  httpd_resp_set_type(req, "text/plain");
  httpd_resp_send(req, "Move command successful", HTTPD_RESP_USE_STRLEN);
  return ESP_OK;
}

esp_err_t test_handler(httpd_req_t *req) {
    Serial.println("Test handler reached");
    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_type(req, "text/plain");
    return httpd_resp_send(req, "Test OK", HTTPD_RESP_USE_STRLEN);
}

esp_err_t options_handler(httpd_req_t *req) {
  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  httpd_resp_set_hdr(req, "Access-Control-Allow-Headers", "Content-Type");

  httpd_resp_set_status(req, "200 OK");
  httpd_resp_send(req, NULL, 0);
  return ESP_OK;
}






