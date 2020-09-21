/**
 * @file touch_driver.c
 */

#include "touch_driver.h"
#include "tp_spi.h"
#include "tp_i2c.h"


void touch_driver_init(void)
{
#if defined (CONFIG_LV_TOUCH_CONTROLLER_XPT2046)
    xpt2046_init();
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_FT6X06)
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_STMPE610)
    stmpe610_init();
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_ADCRAW)
    adcraw_init();
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_FT81X)
    /* nothing to do */
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_RA8875)
    ra8875_touch_init();
#endif
}

bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    bool res = false;

#if defined (CONFIG_LV_TOUCH_CONTROLLER_XPT2046)
    res = xpt2046_read(drv, data);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_FT6X06)
    res = ft6x36_read(drv, data);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_STMPE610)
    res = stmpe610_read(drv, data);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_ADCRAW)
    res = adcraw_read(drv, data);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_FT81X)
    res = FT81x_read(drv, data);
#elif defined (CONFIG_LV_TOUCH_CONTROLLER_RA8875)
    res = ra8875_touch_read(drv, data);
#endif

    return res;
}

