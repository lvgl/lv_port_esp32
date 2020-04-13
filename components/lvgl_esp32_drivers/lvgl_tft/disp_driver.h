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
#endif

/*********************
 *      DEFINES
 *********************/
#define TFT_ORIENTATION_PORTRAIT    0
#define TFT_ORIENTATION_LANDSCAPE   1

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void disp_driver_init(bool init_spi);
void disp_driver_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void disp_driver_rounder(lv_disp_drv_t * disp_drv, lv_area_t * area);


/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DISP_DRIVER_H*/
