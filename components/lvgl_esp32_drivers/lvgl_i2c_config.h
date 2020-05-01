/**
 * @file lvgl_i2c_config.h
 */

#ifndef LVGL_I2C_CONF_H
#define LVGL_I2C_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define DISP_BUF_SIZE (CONFIG_LVGL_DISPLAY_WIDTH * CONFIG_LVGL_DISPLAY_HEIGHT)

#define DISP_I2C_SDA    CONFIG_LVGL_DISP_PIN_SDA
#define DISP_I2C_SCL    CONFIG_LVGL_DISP_PIN_SCL

#define DISP_I2C_ORIENTATION    TFT_ORIENTATION_LANDSCAPE

/**********************
 *      TYPEDEFS
 **********************/


/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LVGL_I2C_CONF_H*/
