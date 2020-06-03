# Is your driver not listed on the supported drivers list?

Consider contribute to this demo project by adding support to your driver.

## Display driver.


## Touch (indev) driver.


## Kconfig and Project Configuration

The ESP32 SDK (ESP-IDF) uses [kconfiglib](https://github.com/ulfalizer/Kconfiglib) which is a Python-based extension to the [Kconfig](https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt) system which provides a compile-time project configuration mechanism. Using `idf.py menuconfig` will update the file sdkconfig and, during build, provide the file sdkconfig.h.

The following options will be defined and can be used in implementations:

- Predefined display configurations section in file `sdkconfig`

  ```
  CONFIG_LVGL_PREDEFINED_DISPLAY_NONE=y
  CONFIG_LVGL_PREDEFINED_DISPLAY_WROVER4=
  CONFIG_LVGL_PREDEFINED_DISPLAY_M5STACK=
  CONFIG_LVGL_PREDEFINED_DISPLAY_M5STICK=
  CONFIG_LVGL_PREDEFINED_DISPLAY_ERTFT0356=
  CONFIG_LVGL_PREDEFINED_DISPLAY_ADA_FEATHERWING=
  CONFIG_LVGL_PREDEFINED_DISPLAY_RPI_MPI3501=
  CONFIG_LVGL_PREDEFINED_DISPLAY_WEMOS_LOLIN=
  ```

In general, only lines with an assigned value will appear in `sdkconfig.h`. For example, the previous block will result in the following single line in `sdkconfig.h`

```
#define CONFIG_LVGL_PREDEFINED_DISPLAY_NONE 1
```

* Display controller, one entry out of the following list

  ```
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9488
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7735S
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
  CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SSD1306
  ```

* Display controller protocol, either SPI or I2C

  ```
  CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI=
  CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_I2C=y
  ```

* Display orientation

  ```
  CONFIG_LVGL_DISPLAY_ORIENTATION_PORTRAIT=
  CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE=y
  ```

* Display width and heigth in px

  ```
  CONFIG_LVGL_DISPLAY_ORIENTATION_PORTRAIT=
  CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE=y
  CONFIG_LVGL_DISPLAY_WIDTH=128
  CONFIG_LVGL_DISPLAY_HEIGHT=64
  ```

* Pins

  ```
  CONFIG_LVGL_DISP_SPI_MOSI=13
  CONFIG_LVGL_DISP_SPI_CLK=14
  CONFIG_LVGL_DISP_SPI_CS=15
  CONFIG_LVGL_DISP_PIN_DC=2
  CONFIG_LVGL_DISP_PIN_RST=4
  CONFIG_LVGL_DISP_PIN_BCKL=27
  CONFIG_LVGL_DISP_PIN_SDA=5
  CONFIG_LVGL_DISP_PIN_SCL=4
  ```

* HSPI and VSPI SPI Bus

  ```
  CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI=
  CONFIG_LVGL_TFT_DISPLAY_SPI_VSPI=
  ```
  
* Other options like

  ```
  CONFIG_LVGL_INVERT_DISPLAY=y
  CONFIG_LVGL_ENABLE_BACKLIGHT_CONTROL=y
  CONFIG_LVGL_BACKLIGHT_ACTIVE_LVL=y
  ```


* The following defines **should not be used** directly because they are only "helper" defines and will probably removed in future versions. Use the defines starting with `CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_` instead.

  ```
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_ILI9341
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_ILI9488
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_ST7789
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_HX8357
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_SH1107
  CONFIG_LVGL_TFT_DISPLAY_USER_CONTROLLER_SSD1306=y
  ```

