#ifndef _APP_REQUEST_HTTPS_H_
#define _APP_REQUEST_HTTPS_H_
#include <stdbool.h>
extern char *output_buffer; // Buffer to store response of http request from event handler
extern bool flag;
void init_https(const char *time);
void parse_json(void);
#endif
