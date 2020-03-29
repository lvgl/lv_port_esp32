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
#include "ili9341.h"
#include "ili9488.h"
#include "st7789.h"
#include "hx8357.h"

/*********************
 *      DEFINES
 *********************/
 /* Add a new define entry at the end for new controllers */
#define TFT_CONTROLLER_ILI9341  0
#define TFT_CONTROLLER_ILI9488  1
#define TFT_CONTROLLER_ST7789	2
#define TFT_CONTROLLER_HX8357   3

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

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DISP_DRIVER_H*/
