#!/bin/bash
set -e

PICO_IP="192.168.5.216"
PICO_MOUNT="/media/pi/RP2350"

echo "=== Regenerating fsdata_custom.c ==="
python3 /home/pi/pico/pico-sdk/src/rp2_common/pico_lwip/tools/makefsdata.py \
    -i fs/index.html fs/style.css fs/script.js \
    -o fsdata_custom.c

echo "=== Checking build configuration ==="
# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Build directory not found. Creating and configuring..."
    mkdir -p build
    cd build
    cmake ..
    cd ..
elif [ "CMakeLists.txt" -nt "build/CMakeCache.txt" ] || [ ! -f "build/CMakeCache.txt" ]; then
    echo "CMakeLists.txt changed or build not configured. Reconfiguring..."
    cd build
    cmake ..
    cd ..
else
    echo "Build configuration is up to date."
fi

echo "=== Building project ==="
cd build
# Make will do incremental builds automatically
make

echo ""
echo "✓ Build complete! pico_web_server.uf2 is ready"
echo ""

# Try picotool first (if Pico is USB-connected and running)
if lsusb | grep -q "2e8a:"; then
    echo "=== Detected Pico on USB, using picotool ==="
    
    # Reboot into BOOTSEL mode
    echo "Rebooting Pico into BOOTSEL mode..."
    if sudo picotool reboot -f -u 2>/dev/null; then
        sleep 2
        
        # Load firmware
        echo "Flashing new firmware..."
        if sudo picotool load -f pico_web_server.uf2; then
            echo "Rebooting Pico..."
            sudo picotool reboot
            
            # Wait and test
            echo "Waiting 10 seconds for boot and WiFi..."
            sleep 10
            
            # Test connection
            echo "=== Testing connection to $PICO_IP ==="
            RETRY=0
            MAX_RETRIES=5
            
            while [ $RETRY -lt $MAX_RETRIES ]; do
                if curl -s --connect-timeout 3 "http://$PICO_IP/index.html" > /dev/null 2>&1; then
                    echo "✓ SUCCESS: Pico web server is responding!"
                    echo ""
                    curl -s "http://$PICO_IP/index.html" | head -10
                    exit 0
                fi
                RETRY=$((RETRY + 1))
                echo "  Attempt $RETRY/$MAX_RETRIES failed, retrying..."
                sleep 2
            done
            
            echo "✗ WARNING: Cannot connect to Pico at $PICO_IP"
            exit 1
        else
            echo "✗ picotool load failed, falling back to manual method"
        fi
    else
        echo "✗ picotool reboot failed (reset interface not enabled?)"
        echo "  Falling back to manual BOOTSEL method..."
    fi
fi

# Fallback: Manual BOOTSEL method
if [ -d "$PICO_MOUNT" ]; then
    echo "=== Deploying via BOOTSEL mount ==="
    cp pico_web_server.uf2 "$PICO_MOUNT/"
    echo "Deployed! Waiting for reboot..."
    sleep 10
    
    echo "=== Testing connection to $PICO_IP ==="
    if curl -s --connect-timeout 5 "http://$PICO_IP/index.html" > /dev/null 2>&1; then
        echo "✓ SUCCESS: Pico web server is responding!"
        curl -s "http://$PICO_IP/index.html" | head -10
    else
        echo "✗ WARNING: Cannot connect to Pico at $PICO_IP"
        exit 1
    fi
else
    echo ""
    echo "Manual deployment required:"
    echo "  1. Hold BOOTSEL button on Pico"
    echo "  2. Plug in USB (or press reset while holding BOOTSEL)"
    echo "  3. Run: cp build/pico_web_server.uf2 /media/pi/RP2350/"
    echo ""
    echo "After flashing the reset-enabled firmware once,"
    echo "future updates can use picotool automatically."
    exit 1
fi
