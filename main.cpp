#include "pico/stdlib.h"
#include "pico/stdio_usb.h"
#include "pico/cyw43_arch.h"
#include "lwip/apps/httpd.h"
#include "pico/time.h"
#include "hardware/pwm.h"
#include <string.h>
#include "wifi_config.h"

// Servo configuration
#define SERVO_PIN 0
#define SERVO_PWM_FREQ 50  // 50Hz for standard servos
#define SERVO_MIN_PULSE 500   // 0.5ms pulse width (microseconds)
#define SERVO_MAX_PULSE 2500  // 2.5ms pulse width (microseconds)
#define SERVO_OFF_PULSE 0     // 0ms to turn off

static bool servo_state = false;

// Forward declarations
void set_servo_position(uint16_t pulse_us);

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

// Initialize PWM for servo control
void init_servo() {
    // Set GPIO function to PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    
    // Find which slice and channel this GPIO is connected to
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // Set PWM frequency to 50Hz (20ms period = 20,000 microseconds)
    // System clock: 125 MHz
    // For 50Hz: period = 1/50 = 0.02s = 20ms
    // We want wrap = 20000 (20ms in microseconds at 1MHz PWM clock)
    // Clock divider: 125MHz / 1MHz = 125
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);  // 1MHz PWM clock
    pwm_config_set_wrap(&config, 20000);     // 20ms period
    
    // Initialize PWM (start disabled)
    pwm_init(slice_num, &config, false);
    
    // Set initial level to 0 (off)
    pwm_set_chan_level(slice_num, channel, 0);
    
    printf("Servo initialized on GPIO %d, slice %d, channel %d\n", SERVO_PIN, slice_num, channel);
}

// Set servo position based on pulse width in microseconds
void set_servo_position(uint16_t pulse_us) {
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // Convert microseconds to PWM level
    // With wrap=20000 and clkdiv=125, PWM clock = 1MHz (1 count = 1 microsecond)
    // So pulse_us directly maps to the PWM level
    uint16_t level = pulse_us;
    
    // Ensure PWM is enabled
    pwm_set_enabled(slice_num, true);
    pwm_set_chan_level(slice_num, channel, level);
    
    printf("Servo position set: %d us (level=%d)\n", pulse_us, level);
}

// Turn servo ON (set to middle position)
void servo_on() {
    servo_state = true;
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    uint channel = pwm_gpio_to_channel(SERVO_PIN);
    
    // Enable PWM first
    pwm_set_enabled(slice_num, true);
    
    // Set to middle position (1.5ms pulse = 1500 microseconds)
    // At 1MHz PWM clock with wrap=20000, level = pulse_width_in_us
    uint16_t level = 1500;
    pwm_set_chan_level(slice_num, channel, level);
    
    printf("Servo ON: level=%d (1.5ms pulse)\n", level);
}

// Turn servo OFF (set to 0 pulse)
void servo_off() {
    servo_state = false;
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    // Disable PWM to stop the signal
    pwm_set_enabled(slice_num, false);
}

// CGI handler for servo control
const char* cgi_servo_handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]) {
    printf("CGI handler called with %d parameters\n", iNumParams);
    
    // Parse the request
    for (int i = 0; i < iNumParams; i++) {
        printf("Param[%d]: %s = %s\n", i, pcParam[i], pcValue[i]);
        if (strcmp(pcParam[i], "action") == 0) {
            if (strcmp(pcValue[i], "on") == 0) {
                printf("Turning servo ON\n");
                servo_on();
                // Blink LED to indicate command received
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                sleep_ms(100);
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                return "/index.html";
            } else if (strcmp(pcValue[i], "off") == 0) {
                printf("Turning servo OFF\n");
                servo_off();
                // Blink LED to indicate command received
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
                sleep_ms(100);
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
                return "/index.html";
            }
        }
    }
    printf("No valid action found\n");
    return "/index.html";
}

// CGI mapping table
const tCGI cgi_handlers[] = {
    {"/servo", cgi_servo_handler},
};

int main() {
    stdio_init_all(); 

    if (cyw43_arch_init()) {
        blink_error();
    }
    
    blink_pattern(1);  // Init OK
    
    // Initialize servo PWM
    init_servo();
    
    // Test servo briefly on startup (optional - comment out if not needed)
    // Uncomment these lines to test if servo hardware works:
    // printf("Testing servo on startup...\n");
    // servo_on();
    // sleep_ms(2000);
    // servo_off();
    
    cyw43_arch_enable_sta_mode();
    
    blink_pattern(2);  // Connecting
    
    if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
        blink_error();
    }
    
    blink_pattern(3);  // Connected
    
    // Initialize HTTP server with CGI support
    httpd_init();
    http_set_cgi_handlers(cgi_handlers, 1);
    
    blink_pattern(4);  // Server started
    
    blink_heartbeat();  // Heartbeat - no need for cyw43_arch_poll in threadsafe mode
    
    return 0;
}

