/**
 * @file disp_driver.c
 */

#include "disp_driver.h"
#include "disp_spi.h"

void disp_driver_init(bool init_spi)
{
#if defined CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI
#pragma message "CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI disp_spi_init"

	if (init_spi) {
		disp_spi_init();
	}
#elif defined CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_I2C
    i2c_master_init();
#endif


#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
    ili9341_init();
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9488
    ili9488_init();
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
    st7789_init();
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
	hx8357_init(HX8357D);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
	sh1107_init();
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SSD1306
    ssd1306_init();
#endif
}

void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
    ili9341_flush(drv, area, color_map);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9488
    ili9488_flush(drv, area, color_map);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
    st7789_flush(drv, area, color_map);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
	hx8357_flush(drv, area, color_map);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
	sh1107_flush(drv, area, color_map);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SSD1306
    ssd1306_flush(drv, area, color_map);
#endif
}

void disp_driver_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area)
{
#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SSD1306
    ssd1306_rounder(disp_drv, area);
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
    sh1107_rounder(disp_drv, area);
#endif
}
