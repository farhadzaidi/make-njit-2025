#ifndef HANDLERS_H
#define HANDLERS_H

esp_err_t stream_handler(httpd_req_t *req);
// esp_err_t move_handler(httpd_req_t *req);
esp_err_t test_handler(httpd_req_t *req);
esp_err_t options_handler(httpd_req_t *req);


#endif