# Raspberry Pi Pico 2W Web Server

A lightweight web server implementation for the Raspberry Pi Pico 2W microcontroller, serving HTML, CSS, and JavaScript files over WiFi using the lwIP HTTP server.

## Features

- 🌐 **HTTP Web Server**: Serves static web content using lwIP HTTP server
- 📡 **WiFi Connectivity**: Connects to WiFi networks using the Pico 2W's built-in WiFi
- 💡 **LED Status Indicators**: Visual feedback through LED blink patterns:
  - 1 blink: Initialization OK
  - 2 blinks: Connecting to WiFi
  - 3 blinks: WiFi connected
  - 4 blinks: HTTP server started
  - Heartbeat: Server running (blinks every 2 seconds)
  - Fast blink: Error state
- 📁 **Embedded Filesystem**: Web files (HTML, CSS, JS) are embedded in the firmware
- 🔄 **Easy Deployment**: Automated deployment script with picotool support

## Requirements

### Hardware
- Raspberry Pi Pico 2W
- USB cable for programming
- WiFi network access

### Software
- Raspberry Pi Pico SDK
- CMake (version 3.13 or higher)
- ARM GCC toolchain
- Python 3 (for filesystem generation)
- picotool (optional, for easy deployment)

## Project Structure

```
.
├── main.cpp              # Main application code
├── CMakeLists.txt        # CMake build configuration
├── lwipopts.h           # lwIP configuration options
├── fsdata_custom.c      # Generated filesystem data (auto-generated)
├── pico_sdk_import.cmake # Pico SDK import
├── deploy.sh            # Deployment script
├── fs/                  # Web files directory
│   ├── index.html      # Main HTML page
│   ├── style.css       # Stylesheet
│   └── script.js       # JavaScript
└── build/               # Build output directory
```

## Setup

### 1. Install Pico SDK

Ensure the Raspberry Pi Pico SDK is installed and set the `PICO_SDK_PATH` environment variable:

```bash
export PICO_SDK_PATH=/home/pi/pico/pico-sdk
```

### 2. Configure WiFi Credentials

Edit `main.cpp` and update the WiFi credentials:

```cpp
const char WIFI_SSID[] = "YourWiFiNetwork";
const char WIFI_PASSWORD[] = "YourPassword";
```

### 3. Configure Pico IP Address (Optional)

If you want to test the connection automatically, edit `deploy.sh` and update the IP address:

```bash
PICO_IP="192.168.1.xxx"  # Your Pico's IP address
```

The Pico will obtain an IP address via DHCP. Check your router's DHCP client list or use a network scanner to find the assigned IP.

## Building

### Manual Build

1. Create and enter the build directory:
```bash
mkdir -p build
cd build
```

2. Configure with CMake:
```bash
cmake ..
```

3. Build the project:
```bash
make
```

4. Generate filesystem data:
```bash
cd ..
python3 $PICO_SDK_PATH/src/rp2_common/pico_lwip/tools/makefsdata.py \
    -i fs/index.html fs/style.css fs/script.js \
    -o fsdata_custom.c
```

5. Rebuild to include filesystem:
```bash
cd build
make
```

The compiled firmware will be available as `build/pico_web_server.uf2`.

### Automated Build and Deploy

Use the provided deployment script which handles filesystem generation, building, and deployment:

```bash
./deploy.sh
```

The script will:
1. Regenerate `fsdata_custom.c` from web files
2. Build the project
3. Attempt to deploy via picotool (if Pico is USB-connected)
4. Fall back to manual BOOTSEL method if needed
5. Test the connection (if IP is configured)

## Deployment

### Method 1: Using picotool (Recommended)

If you've flashed the firmware once with reset interface enabled, you can use picotool:

```bash
sudo picotool load -f build/pico_web_server.uf2
sudo picotool reboot
```

### Method 2: BOOTSEL Mode

1. Hold the BOOTSEL button on the Pico
2. Connect USB (or press reset while holding BOOTSEL)
3. Copy the firmware:
```bash
cp build/pico_web_server.uf2 /media/pi/RP2350/
```

The Pico will automatically reboot and start the web server.

## Usage

1. Power on the Pico 2W
2. Wait for the LED blink sequence:
   - 1 blink: Initialization
   - 2 blinks: Connecting to WiFi
   - 3 blinks: Connected
   - 4 blinks: Server started
   - Heartbeat: Running (blinks every 2 seconds)
3. Find the Pico's IP address (check router DHCP list or use network scanner)
4. Open a web browser and navigate to: `http://<pico-ip-address>/index.html`

## Customizing Web Content

1. Edit files in the `fs/` directory:
   - `fs/index.html` - Main HTML page
   - `fs/style.css` - Stylesheet
   - `fs/script.js` - JavaScript

2. Regenerate the filesystem data:
```bash
python3 $PICO_SDK_PATH/src/rp2_common/pico_lwip/tools/makefsdata.py \
    -i fs/index.html fs/style.css fs/script.js \
    -o fsdata_custom.c
```

3. Rebuild the project:
```bash
cd build
make
```

4. Deploy the new firmware

## Troubleshooting

### LED Blinks Rapidly (Error State)
- WiFi credentials may be incorrect
- WiFi network may be out of range
- Check serial output via USB for detailed error messages

### Cannot Connect to Web Server
- Verify the Pico is connected to WiFi (3 blinks completed)
- Check that the server started (4 blinks completed)
- Verify the IP address is correct
- Ensure your device is on the same network
- Check firewall settings

### Build Errors
- Verify `PICO_SDK_PATH` is set correctly
- Ensure all dependencies are installed
- Try cleaning the build directory: `rm -rf build && mkdir build`

## Technical Details

- **Network Stack**: lwIP (lightweight IP)
- **HTTP Server**: lwIP HTTP daemon
- **WiFi Driver**: CYW43 (threadsafe background mode)
- **Filesystem**: Embedded filesystem data in C array format
- **Build System**: CMake
- **Compiler**: ARM GCC

## License

This project is provided as-is for educational and personal use.

## Author

Rturo Treviso

