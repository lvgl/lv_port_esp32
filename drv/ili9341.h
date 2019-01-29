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
#include "../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

#define ILI9341_DC   19
#define ILI9341_RST  18
#define ILI9341_BCKL 23

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void ili9431_init(void);
void ili9431_fill(int32_t x1, int32_t y1, int32_t x2, int32_t y2, lv_color_t color);
void ili9431_flush(int32_t x1, int32_t y1, int32_t x2, int32_t y2, const lv_color_t * color_map);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*ILI9341_H*/
