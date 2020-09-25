# Is your driver not listed on the supported drivers list?

Consider contributing to this demo project by adding support to your driver.

## Display driver.

To enable LVGL to work with your display you would need to implement from one up to three callback functions (one function for RGB TFT displays, three functions for monochrome display controllers), you can add more functions to enable backlight control, etc.

All display controller code is stored on the `components/lvgl_esp32_drivers/lvgl_tft` directory, see `disp_driver` and `CMakeLists.txt` to add your code into the idf project.

Create a header and source file named after the display controller (in lowercase). On the header file declare the necessary functions, such as:

```c
/* Configure your display */
void x_init(void);

/* LVGL callbacks */
void x_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
/* Only for monochrome displays */
void x_rounder(lv_disp_drv_t *disp_drv, lv_area_t *area);
void x_set_px(lv_disp_drv_t *disp_drv, uint8_t *buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y, lv_color_t color, lv_opa_t opa);
```

Where x is the controller name.

For more information on the function callbacks check LVGL documentation: (Display driver)[https://docs.lvgl.io/v7/en/html/porting/display.html#display-driver].

Add your display functions on `disp_driver_init`, `disp_driver_flush`, `disp_driver_rounder` and `disp_driver_set_px` on the `disp_driver.c` file.

## Touch (indev) driver.

To enable LVGL to work with your touch controller you would need to implement an initialization function and one function to get the data out from your touch controller.

All touch controller code is stored on the `components/lvgl_esp32_drivers/lvgl_touch` directory.

Create a header and source file named after the display controller (in lowercase). On the header file declare the necessary functions, such as:

```c
/* Configure your display */
void x_init(void);

/* LVGL callbacks */
bool x_read(lv_disp_drv_t *drv, lv_indev_data_t *data);
```

Where x is the controller name.

For more information on the function callbacks check LVGL documentation: (Display driver)[https://docs.lvgl.io/v7/en/html/porting/indev.html].

## Kconfig and Project Configuration

The ESP32 SDK (ESP-IDF) uses [kconfiglib](https://github.com/ulfalizer/Kconfiglib) which is a Python-based extension to the [Kconfig](https://www.kernel.org/doc/Documentation/kbuild/kconfig-language.txt) system which provides a compile-time project configuration mechanism. Using `idf.py menuconfig` will update the file sdkconfig and, during build, provide the file sdkconfig.h.

The following options will be defined and can be used in implementations:

- Predefined display configurations section in file `sdkconfig`

  ```
  CONFIG_LV_PREDEFINED_DISPLAY_NONE=y
  CONFIG_LV_PREDEFINED_DISPLAY_WROVER4=
  CONFIG_LV_PREDEFINED_DISPLAY_M5STACK=
  CONFIG_LV_PREDEFINED_DISPLAY_M5STICK=
  CONFIG_LV_PREDEFINED_DISPLAY_ERTFT0356=
  CONFIG_LV_PREDEFINED_DISPLAY_ADA_FEATHERWING=
  CONFIG_LV_PREDEFINED_DISPLAY_RPI_MPI3501=
  CONFIG_LV_PREDEFINED_DISPLAY_WEMOS_LOLIN=
  ```

In general, only lines with an assigned value will appear in `sdkconfig.h`. For example, the previous block will result in the following single line in `sdkconfig.h`

```
#define CONFIG_LV_PREDEFINED_DISPLAY_NONE 1
```

* Display controller, one entry out of the following list

  ```
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9341
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_ILI9488
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_ST7789
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_ST7735S
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_HX8357
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_SH1107
  CONFIG_LV_TFT_DISPLAY_CONTROLLER_SSD1306
  ```

* Display controller protocol, either SPI or I2C

  ```
  CONFIG_LV_TFT_DISPLAY_PROTOCOL_SPI=
  CONFIG_LV_TFT_DISPLAY_PROTOCOL_I2C=y
  ```

* Display orientation

  ```
  CONFIG_LV_DISPLAY_ORIENTATION_PORTRAIT=
  CONFIG_LV_DISPLAY_ORIENTATION_LANDSCAPE=y
  ```

* Display width and heigth in px

  ```
  CONFIG_LV_DISPLAY_ORIENTATION_PORTRAIT=
  CONFIG_LV_DISPLAY_ORIENTATION_LANDSCAPE=y
  CONFIG_LV_DISPLAY_WIDTH=128
  CONFIG_LV_DISPLAY_HEIGHT=64
  ```

* Pins

  ```
  CONFIG_LV_DISP_SPI_MOSI=13
  CONFIG_LV_DISP_SPI_CLK=14
  CONFIG_LV_DISP_SPI_CS=15
  CONFIG_LV_DISP_PIN_DC=2
  CONFIG_LV_DISP_PIN_RST=4
  CONFIG_LV_DISP_PIN_BCKL=27
  CONFIG_LV_DISP_PIN_SDA=5
  CONFIG_LV_DISP_PIN_SCL=4
  ```

* HSPI and VSPI SPI Bus

  ```
  CONFIG_LV_TFT_DISPLAY_SPI_HSPI=
  CONFIG_LV_TFT_DISPLAY_SPI_VSPI=
  ```
  
* Other options like

  ```
  CONFIG_LV_INVERT_DISPLAY=y
  CONFIG_LV_ENABLE_BACKLIGHT_CONTROL=y
  CONFIG_LV_BACKLIGHT_ACTIVE_LVL=y
  ```


* The following defines **should not be used** directly because they are only "helper" defines and will probably removed in future versions. Use the defines starting with `CONFIG_LV_TFT_DISPLAY_CONTROLLER_` instead.

  ```
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_ILI9341
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_ILI9488
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_ST7789
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_HX8357
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_SH1107
  CONFIG_LV_TFT_DISPLAY_USER_CONTROLLER_SSD1306=y
  ```

