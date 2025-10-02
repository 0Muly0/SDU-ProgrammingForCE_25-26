#include <cstdio>
#include "pico/stdlib.h"
// 4. Included pico_httpd_h which carries the pico_httpd_start function
#include "pico_httpd.h"

// 3. Defined new main, where we call pico_httpd_start function (old main)
int main() {
    int ret = pico_httpd_start();
    if(ret != 0) {
        printf("HTTP server failed to start \n");
        return ret;
    }
    return 0;
}