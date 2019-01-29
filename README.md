# LittlevGL project for ESP32

![Example GUI with LittelvGL on ESP32](https://raw.githubusercontent.com/littlevgl/esp32_ili9431/master/screenshot.jpg)


## Get started 
### Install the ESP32 SDK
1. Install ESP-IDF: http://esp-idf.readthedocs.io/en/latest/
2. Get this projects: `git clone https://github.com/littlevgl/esp32_ili9431.git --recurse-submodules

### Add LittelvGL to the build
To link LittlevGL (lvgl) and lv_examples with ESP-IDF you need to add a **component.mk** file to each directory. Next to this REAMDE file you find two example component.mk files
- lvgl_component.mk
- lv_example_component.mk
Rename them to component.mk and copy to the lvgl and lv_examples directories.

### Flash to ESP32
1. If you are not in the project's diectory: `cd esp32_ili9431`
2. `make`
3. `make flash`

## Drivers
This project comes with an **ILI9341** display driver and an **XPT2046** resistive touchpad driver. Both devices are communicating via SPI.

### ILI9341
The default pinout is:

| Name | Pin |
|------|-----|
| MOSI | 13 |
| SCLK | 14 |
| CS | 5 |
| DC | 19 |
| BCKL | 23 |
| RST | 18 | 

You can modify the pin configuration in `drv/disp_spi.h` and `drv/ili9341.h`

For ILI9341 HSPI is used.


### XPT2046

The default pinout is

| Name | Pin |
|------|-----|
| MOSI | 32 |
| MISO | 25 |
| SCLK | 26 |
| CS | 33 |
| IRQ | 25 |

You can modify the pin configuration in `drv/tp_spi.h` and `drv/spt2046.h`

For XPT2046 VSPI is used

