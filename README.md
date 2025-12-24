# Raspberry Pi Pico 2W Web Server

A lightweight web server implementation for the Raspberry Pi Pico 2W microcontroller, written in C++. Serves HTML, CSS, and JavaScript files over WiFi using the lwIP HTTP server.

## Features

- 🌐 **HTTP Web Server**: Serves static web content using lwIP HTTP server
- 📡 **WiFi Connectivity**: Connects to WiFi networks using the Pico 2W's built-in WiFi
- 🎛️ **Servo Motor Control**: Web-based servo control with on/off toggle switch
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
- Servo motor (optional, for servo control functionality)
  - Standard servo motor (e.g., SG90, MG90S)
  - External 5V power supply (recommended for servos that draw high current)

### Software
- Raspberry Pi Pico SDK
- CMake (version 3.13 or higher)
- ARM GCC toolchain (C++17 support)
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

### 4. Servo Motor Wiring (Optional)

If you want to use the servo control feature, connect a servo motor as follows:

**Standard Servo Motor Connections:**
- **Red wire (VCC/5V)** → VSYS (pin 39) or VBUS (pin 40)
  - *Note: For servos that draw high current, use an external 5V power supply and connect its ground to Pico's GND*
- **Black/Brown wire (GND)** → Any GND pin (e.g., pin 38, 3, 8, 13, 18, 23, 28, 33)
- **Orange/Yellow wire (Signal)** → GPIO 16 (physical pin 21)

**Power Considerations:**
- Small servos (like SG90) may work directly from VSYS/VBUS
- Larger servos should use an external 5V power supply
- Always connect the external power supply's ground to the Pico's GND (common ground)

**To use a different GPIO pin**, edit `main.cpp` and change:
```cpp
#define SERVO_PIN 16  // Change to desired GPIO pin (0, 1, 2, etc.)
```

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

### Servo Control

The web interface provides a servo control page with an on/off toggle switch:

1. **Toggle Servo ON**: Click the switch to turn it ON
   - Servo moves to middle position (1.5ms pulse width)
   - Status displays "Servo: ON" in green
   - LED on Pico blinks briefly to confirm command received

2. **Toggle Servo OFF**: Click the switch again to turn it OFF
   - Servo stops receiving PWM signal
   - Status displays "Servo: OFF"
   - LED on Pico blinks briefly to confirm command received

**Note**: The servo control uses CGI (Common Gateway Interface) to communicate between the web page and the Pico. Commands are sent via HTTP GET requests to `/servo?action=on` or `/servo?action=off`.

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

### Servo Not Responding
- **Check wiring**: Verify all three wires are connected correctly
  - Red wire to VSYS/VBUS or external 5V supply
  - Black/Brown wire to GND
  - Orange/Yellow wire to GPIO 16 (or configured pin)
- **Check power**: Ensure servo is receiving adequate power
  - Small servos may work from VSYS/VBUS
  - Larger servos need external 5V power supply
  - Verify external power supply ground is connected to Pico GND
- **Check LED feedback**: When toggling the switch, the Pico LED should blink briefly
  - If LED doesn't blink, the CGI handler may not be receiving requests
  - Check browser console (F12) for JavaScript errors
- **Check serial output**: Connect via USB and monitor serial output:
  ```bash
  picotool reboot -f -u
  minicom -o -D /dev/ttyACM0 -b 115200
  ```
  - Look for "CGI handler called" messages when toggling
  - Look for "Servo ON" or "Servo OFF" messages
- **Try different GPIO pin**: Change `SERVO_PIN` in `main.cpp` to GPIO 0, 1, or 2
- **Verify servo works**: Test servo with a simple PWM test program to verify hardware
- **Check pulse width**: Servo may need different pulse width - edit `servo_on()` in `main.cpp`:
  - Try `set_servo_position(1000);` for left position
  - Try `set_servo_position(2000);` for right position
  - Standard range is 500-2500 microseconds

## Technical Details

- **Programming Language**: C++ (C++17 standard)
- **Network Stack**: lwIP (lightweight IP)
- **HTTP Server**: lwIP HTTP daemon with CGI support
- **WiFi Driver**: CYW43 (threadsafe background mode)
- **Filesystem**: Embedded filesystem data in C array format
- **Servo Control**: PWM-based servo control using hardware PWM
  - Default GPIO: GPIO 16 (configurable)
  - PWM Frequency: 50Hz (20ms period)
  - Pulse Width Range: 500-2500 microseconds (0.5ms - 2.5ms)
  - Default ON position: 1500 microseconds (middle/neutral)
- **Build System**: CMake
- **Compiler**: ARM GCC

## License

This project is provided as-is for educational and personal use.

## Author

Rturo Treviso

## Todo List

- [ ] Improve Security Access (SSID PASSWORD protection)
