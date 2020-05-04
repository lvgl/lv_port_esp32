/**
 * @file ili9486.h
 *
 */

#ifndef ILI9486_H
#define ILI9486_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

#include "lvgl/lvgl.h"
#include "../lvgl_driver.h"

/*********************
 *      DEFINES
 *********************/
#define ILI9486_DC   CONFIG_LVGL_DISP_PIN_DC
#define ILI9486_RST  CONFIG_LVGL_DISP_PIN_RST
#define ILI9486_BCKL CONFIG_LVGL_DISP_PIN_BCKL

#define ILI9486_ENABLE_BACKLIGHT_CONTROL CONFIG_LVGL_ENABLE_BACKLIGHT_CONTROL

#if CONFIG_LVGL_BACKLIGHT_ACTIVE_LVL
  #define ILI9486_BCKL_ACTIVE_LVL 1
#else
  #define ILI9486_BCKL_ACTIVE_LVL 0
#endif

// if text/images are backwards, try setting this to 1
#define ILI9486_INVERT_DISPLAY CONFIG_LVGL_INVERT_DISPLAY

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void ili9486_init(void);
void ili9486_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void ili9486_enable_backlight(bool backlight);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ILI9486_H*/
