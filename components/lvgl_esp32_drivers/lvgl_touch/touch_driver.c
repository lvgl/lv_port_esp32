/**
 * @file touch_driver.c
 */

#include "touch_driver.h"
#include "tp_spi.h"
#include "tp_i2c.h"


void touch_driver_init(void)
{
#if CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_XPT2046
    xpt2046_init();
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_FT6X06
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_STMPE610
    stmpe610_init();
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_ADCRAW
    adcraw_init();
#endif
}

bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    bool res = false;

#if CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_XPT2046
    res = xpt2046_read(drv, data);
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_FT6X06
    res = ft6x36_read(drv, data);
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_STMPE610
	res = stmpe610_read(drv, data);
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_ADCRAW
    res = adcraw_read(drv, data);
#endif

    return res;
}

