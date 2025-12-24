#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"

const char WIFI_SSID[] = "Treviso";
const char WIFI_PASSWORD[] = "Garbin@1921";

void blink_pattern(int count) {
    for(int i = 0; i < count; i++) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(200);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(200);
    }
    sleep_ms(1000);
}

void blink_error() {
    while(1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(100);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(100);
    }
}

void blink_heartbeat() {
 while(1) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(50);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(2000);
    }
}


int main() {
    stdio_init_all(); 

    if (cyw43_arch_init()) {
        blink_error();
    }
    
    blink_pattern(1);  // Init OK
    
    cyw43_arch_enable_sta_mode();
    
    blink_pattern(2);  // Connecting
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        blink_error();
    }
    
    blink_pattern(3);  // Connected
    
    httpd_init();
    
    blink_pattern(4);  // Server started
    
    blink_heartbeat();  // Heartbeat - no need for cyw43_arch_poll in threadsafe mode
    
    return 0;
}

