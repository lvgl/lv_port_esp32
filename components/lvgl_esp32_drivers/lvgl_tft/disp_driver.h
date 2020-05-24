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

#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
#include "ili9341.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9488
#include "ili9488.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
#include "st7789.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
#include "hx8357.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9486
#include "ili9486.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
#include "sh1107.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SSD1306
#include "ssd1306.h"
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_FT81X
#include "FT81x.h"
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
void disp_driver_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area);
void disp_driver_set_px(struct _disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
    lv_color_t color, lv_opa_t opa);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DISP_DRIVER_H*/
