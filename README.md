# LittlevGL project for ESP32

![Example GUI with LittlevGL on ESP32](screenshot.jpg)


## Get started 
### Install the ESP32 SDK
1. Install ESP-IDF: http://esp-idf.readthedocs.io/en/latest/
2. Get this projects: `git clone --recurse-submodules https://github.com/littlevgl/esp32_ili9431.git`

### Add LittlevGL to the build
To link LittlevGL (lvgl) and lv_examples with ESP-IDF you need to add a **component.mk** file to each directory.

Next to this README file you find two component.mk files:
- lvgl_component.mk
- lv_example_component.mk

Rename lvgl_component.mk to component.mk and move it to the lvgl directory, then rename lv_example_component.mk to component.mk and move it to the lv_examples directory.

## Assign the correct pinout depending on your ESP32 dev board
There are several development boards based on the ESP32 chip, make sure you assign the correct pin numbers to the signals that interface with the TFT display board, below are some examples:

### Flash to ESP32
1. If you are not in the project's directory: `cd esp32_ili9431`
2. Build the project: `make`
3. Upload the application to your board: `make flash`

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
| MISO | 35 |
| SCLK | 26 |
| CS | 33 |
| IRQ | 25 |

You can modify the pin configuration in `drv/tp_spi.h` and `drv/spt2046.h`

For XPT2046 VSPI is used

## ESP32 DevKit v1 with 30 GPIOS

There is another development kit with only 30 GPIOs available:

![LittlevGL on ESP32 Kit with 30 GPIO](ESP32_DevkitV1_30_GPIO.png)

### ILI9341

For ILI9341 HSPI is used, modify the pin configuration in `drv/disp_spi.h` to:

```c
#define DISP_SPI_MOSI 13
#define DISP_SPI_CLK  14
#define DISP_SPI_CS   15
```

and `drv/ili9341.h` to:
```c
#define ILI9341_DC   2
#define ILI9341_RST  4
#define ILI9341_BCKL 21
```

### XPT2046

For XPT2046 VSPI is used, modify the pin configuration in `drv/tp_spi.h` to:

```c
#define TP_SPI_MOSI 23
#define TP_SPI_MISO 19
#define TP_SPI_CLK  18
#define TP_SPI_CS   5
```

and `drv/xpt2046.h` to:
```c
#define XPT2046_IRQ 25
```
