#ifndef PICO_HTTPD_H
#define PICO_HTTPD_H

// 1. Defined a new file which declares two new functions
#ifdef __cplusplus
extern "C" {
#endif

// New main function to substitute old main
int pico_httpd_start(void);

void pico_httpd_set_led(bool on);

#ifdef __cplusplus
}
#endif
#endif