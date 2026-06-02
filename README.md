# RP2040TouchAdvent - Advent Calendar based on the RP2040-Touch-LCD-1.28
This is an Advent calendar project based on the [Waveshare RP2040-Touch-LCD-1.28](https://www.waveshare.com/wiki/RP2040-Touch-LCD-1.28),
a DS3231 RTC and some WS2812B LEDs. This goes with a demo of the build on my YouTube channel [DrJonEA](https://youtube.com/@drjonea).

## Cloning
This project using submodules please close with the option git clone --recurse-submodules

## Build process
This project requires the Pico SDK to be installed and the PICO_SDK_PATH environment variable to be set appropriately. I've tested with SDK 2.0.0

Build process is the normal Pico build process and should also build within the VSCode Pico Extension.
```
mkdir build
cd build
cmake ..
make
```
Then use Bootsel to flash the RP2040 device.

## Connections

+ GPIO18 is used to drive the WS2812B LED (33 of them). A level shifter between 3V3 and 5V should be on the GPIO18 line between this and the input to the WS2812B chain
+ WS2812B LED will require 5V which is VSYS on the Touches output 
+ GPIO28 pull to ground latching switch to turn on demo mode
+ GPIO27 A LED with  100Ohm resister from GPIO27 to LED and then to Ground
+ GPIO16 is SDA for the RTC
+ GPIO17 is SCL for the RTC
+ RTC will require 5V which is ADC_AVDD on the Touch output connector
