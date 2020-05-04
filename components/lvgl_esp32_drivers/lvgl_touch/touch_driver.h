/**
 * @file touch_driver.h
 */

#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
#include "lvgl/lvgl.h"
#include "xpt2046.h"
#include "ft6x36.h"
#include "stmpe610.h"
#include "adcraw.h"

/*********************
*      DEFINES
*********************/
/* Add a new define entry at the end for new controllers */
#define TOUCH_CONTROLLER_NONE	    0
#define TOUCH_CONTROLLER_XPT2046    1
#define TOUCH_CONTROLLER_FT6X06	    2
#define TOUCH_CONTROLLER_STMPE610   3
#define TOUCH_CONTROLLER_ADCRAW     4

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void touch_driver_init(bool init_spi);
bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TOUCH_DRIVER_H */

