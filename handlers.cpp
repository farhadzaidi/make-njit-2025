#include "esp_err.h"
#include "esp_http_server.h"
#include "esp_camera.h"

// Use this endpoint to stream frames using MJPEG
esp_err_t stream_handler(httpd_req_t *req) {
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