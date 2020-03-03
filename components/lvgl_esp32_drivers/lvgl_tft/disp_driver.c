/**
 * @file disp_driver.c
 */

#include "disp_driver.h"
#include "disp_spi.h"

void disp_driver_init(bool init_spi)
{
	if (init_spi) {
		disp_spi_init();
	}
	
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9341
    ili9341_init();
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9488
    ili9488_init();
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_HX8357
	hx8357_init(HX8357D);
#endif
}

void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9341
    ili9341_flush(drv, area, color_map);
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9488
    ili9488_flush(drv, area, color_map);
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_HX8357
	hx8357_flush(drv, area, color_map);
#endif
}
