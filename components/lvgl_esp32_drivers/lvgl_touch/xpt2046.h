/**
 * @file XPT2046.h
 *
 */

#ifndef XPT2046_H
#define XPT2046_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include <stdint.h>
#include <stdbool.h>
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define XPT2046_IRQ CONFIG_LVGL_TOUCH_PIN_IRQ

#define XPT2046_AVG 4
#define XPT2046_X_MIN       CONFIG_LVGL_TOUCH_X_MIN
#define XPT2046_Y_MIN       CONFIG_LVGL_TOUCH_Y_MIN
#define XPT2046_X_MAX       CONFIG_LVGL_TOUCH_X_MAX
#define XPT2046_Y_MAX       CONFIG_LVGL_TOUCH_Y_MAX
#define XPT2046_X_INV       CONFIG_LVGL_TOUCH_INVERT_X
#define XPT2046_Y_INV       CONFIG_LVGL_TOUCH_INVERT_Y
#define XPT2046_XY_SWAP		CONFIG_LVGL_TOUCH_XY_SWAP

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void xpt2046_init(void);
bool xpt2046_read(lv_indev_drv_t * drv, lv_indev_data_t * data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* XPT2046_H */
