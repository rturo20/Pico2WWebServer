cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_web_server C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico_web_server
    main.cpp
)

target_include_directories(pico_web_server PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(pico_web_server
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
)

pico_enable_stdio_usb(pico_web_server 1)
pico_enable_stdio_uart(pico_web_server 0)

# Enable picotool reset without BOOTSEL button
target_compile_definitions(pico_web_server PRIVATE
    PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=1
    PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1
)

pico_add_extra_outputs(pico_web_server)
