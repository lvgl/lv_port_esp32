/**
 * @file touch_driver.h
 */

#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TOUCH_CONTROLLER_NONE	    0
#define TOUCH_CONTROLLER_XPT2046    1
#define TOUCH_CONTROLLER_FT6X06	    2

#if CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_XPT2046
#include "xpt2046.h"
#elif CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_FT6X06
#include "ft6x36.h"
#endif

#include "lvgl/lvgl.h"

#include "tp_spi.h"
#include "tp_i2c.h"

void touch_driver_init(void);
bool touch_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TOUCH_DRIVER_H */

