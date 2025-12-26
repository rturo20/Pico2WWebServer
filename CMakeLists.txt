# Minimum CMake version required (3.13 is needed for Pico SDK)
cmake_minimum_required(VERSION 3.13)

# Import the Pico SDK configuration
# This file should be copied from the Pico SDK examples
include(pico_sdk_import.cmake)

# Set board to Pico 2W (must be before pico_sdk_init)
# This is required for WiFi support on Pico 2W
set(PICO_BOARD pico2_w CACHE STRING "Pico board" FORCE)

# Define the project name and supported languages
# C, CXX (C++), and ASM (Assembly) are required for Pico development
project(pico_web_server C CXX ASM)

# Set C standard to C11
set(CMAKE_C_STANDARD 11)
# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Initialize the Pico SDK
# This sets up all the necessary paths and configurations for Pico development
pico_sdk_init()

# Create the executable target
# This defines the main output binary that will be built
add_executable(pico_web_server
    main.cpp
)

# Add include directories for header files
# ${CMAKE_CURRENT_LIST_DIR} refers to the directory containing this CMakeLists.txt
# This allows the code to include local headers like wifi_config.h
target_include_directories(pico_web_server PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link required Pico SDK libraries
# pico_stdlib: Standard library functions (stdio, sleep, etc.)
# pico_cyw43_arch_lwip_threadsafe_background: WiFi driver with lwIP in threadsafe background mode
# pico_lwip_http: lwIP HTTP server implementation
target_link_libraries(pico_web_server
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    pico_lwip_http
)

# Enable USB stdio (for serial communication over USB)
# Set to 1 to enable, 0 to disable
pico_enable_stdio_usb(pico_web_server 1)
# Disable UART stdio (we're using USB instead)
pico_enable_stdio_uart(pico_web_server 0)

# Enable picotool reset without BOOTSEL button
# These flags allow picotool to reset the Pico into BOOTSEL mode programmatically
# This enables remote firmware updates without manually pressing the BOOTSEL button
target_compile_definitions(pico_web_server PRIVATE
    PICO_STDIO_USB_ENABLE_RESET_VIA_BAUD_RATE=1
    PICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=1
)

# Generate additional output formats
# This creates .uf2, .hex, .bin, and .dis files in addition to .elf
# .uf2 is the format used for flashing via BOOTSEL mode
pico_add_extra_outputs(pico_web_server)
