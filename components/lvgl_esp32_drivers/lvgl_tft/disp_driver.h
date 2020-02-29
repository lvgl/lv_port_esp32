/**
 * @file disp_driver.h
 */

#ifndef DISP_DRIVER_H
#define DISP_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

#include "lvgl/lvgl.h"

/* Add a new enum entry at the end for new controllers */
enum {
    TFT_CONTROLLER_ILI9341  = 0,
    TFT_CONTROLLER_ILI9488  = 1,
};

#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9341
#include "ili9341.h"
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9488
#include "ili9488.h"
#endif

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void disp_driver_init(void);
void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DISP_DRIVER_H*/
