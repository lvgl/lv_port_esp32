#include "touch_driver.h"

void touch_driver_init(void)
{
#if CONFIG_TOUCH_CONTROLLER == TOUCH_CONTROLLER_XPT2046
    tp_spi_init();
    xpt2046_init();
#elif CONFIG_TOUCH_CONTROLLER == TOUCH_CONTROLLER_FT6X06
    ft6x36_init(FT6236_I2C_SLAVE_ADDR);
#endif
}

bool touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
    bool res = false;

#if CONFIG_TOUCH_CONTROLLER == TOUCH_CONTROLLER_XPT2046
    res = xpt2046_read(drv, data);
#elif CONFIG_TOUCH_CONTROLLER == TOUCH_CONTROLLER_FT6X06
    res = ft6x36_read(drv, data);
#endif

    return res;
}

