/**
 * @file lv_templ.h
 *
 */

#ifndef ILI9341_H
#define ILI9341_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

#include "lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/
#define DISP_BUF_SIZE (LV_HOR_RES_MAX * 40)
#define ILI9341_DC   CONFIG_LVGL_DISP_PIN_DC
#define ILI9341_RST  CONFIG_LVGL_DISP_PIN_RST
#define ILI9341_BCKL CONFIG_LVGL_DISP_PIN_BCKL

#define ILI9341_ENABLE_BACKLIGHT_CONTROL CONFIG_LVGL_ENABLE_BACKLIGHT_CONTROL

#if CONFIG_LVGL_BACKLIGHT_ACTIVE_LVL
  #define ILI9341_BCKL_ACTIVE_LVL 1
#else
  #define ILI9341_BCKL_ACTIVE_LVL 0
#endif

// if text/images are backwards, try setting this to 1
#define ILI9341_INVERT_DISPLAY CONFIG_LVGL_INVERT_DISPLAY

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void ili9341_init(void);
void ili9341_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void ili9341_enable_backlight(bool backlight);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ILI9341_H*/
