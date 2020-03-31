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

#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 0
#include "ili9341.h"
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 1
#include "ili9488.h"
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 2
#include "st7789.h"
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 3
#include "hx8357.h"
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == 4
#include "ssd1306.h"
#endif

/*********************
 *      DEFINES
 *********************/
 /* Add a new define entry at the end for new controllers */
#define TFT_CONTROLLER_ILI9341  0
#define TFT_CONTROLLER_ILI9488  1
#define TFT_CONTROLLER_ST7789	2
#define TFT_CONTROLLER_HX8357   3
#define TFT_CONTROLLER_SSD1306  4

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
