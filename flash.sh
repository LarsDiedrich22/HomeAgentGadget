#!/bin/bash
set -e

export PICO_SDK_PATH=~/pico/pico-sdk
PROJECT_DIR="/Users/lars/Documents/HF/Git/RP2040TouchAdvent"

echo "=== Building RP2040 Advent Project ==="
cd "$PROJECT_DIR"
rm -rf build
mkdir build
cd build
cmake .. > /dev/null
make -j4

echo ""
echo "✅ Build complete!"
UF2_FILE="$PROJECT_DIR/build/src/Advent.uf2"
ls -lh "$UF2_FILE"

echo ""
echo "=== Flashing to RP2040 ==="
PICO_MOUNT=$(mount | grep -i "pico\|rpi" | awk '{print $3}' | head -1)

if [ -z "$PICO_MOUNT" ]; then
    echo "❌ RP2040 not found in bootloader mode!"
    echo "   1. Hold BOOTSEL button"
    echo "   2. Plug in USB-C cable"
    echo "   3. Release button"
    echo "   4. Run this script again"
    exit 1
fi

echo "Found RP2040 at: $PICO_MOUNT"
cp "$UF2_FILE" "$PICO_MOUNT/"
sleep 2

echo "✅ Flashed successfully! Device rebooting..."
echo "   Your Advent calendar is now running!"
