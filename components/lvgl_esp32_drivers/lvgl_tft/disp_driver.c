/**
 * @file disp_driver.c
 */

#include "disp_driver.h"

#include "ili9341.h"
#include "ili9488.h"
#include "hx8357.h"

void disp_driver_init(void)
{
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 0
    ili9341_init();
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 1
    ili9488_init();
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 2
	hx8357_init(HX8357D);
#endif
}

void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 0
    ili9341_flush(drv, area, color_map);
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 1
    ili9488_flush(drv, area, color_map);
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 2
	hx8357_flush(drv, area, color_map);
#endif
}
