/**
 * @file lv_templ.h
 *
 */

#ifndef SSD1306_H
#define SSD1306_H

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
#define DISP_BUF_SIZE   (CONFIG_LVGL_DISPLAY_WIDTH*CONFIG_LVGL_DISPLAY_HEIGHT)

#define SSD1306_SDA     CONFIG_LVGL_DISP_PIN_SDA
#define SSD1306_SCL     CONFIG_LVGL_DISP_PIN_SCL
#define SSD1306_DISPLAY_ORIENTATION     TFT_ORIENTATION_LANDSCAPE

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

void i2c_master_init(void);
void ssd1306_init(void);
void ssd1306_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void ssd1306_rounder(struct _disp_drv_t * disp_drv, lv_area_t *area);
void ssd1306_set_px_cb(struct _disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
    lv_color_t color, lv_opa_t opa);

void ssd1306_display_on(void);
void ssd1306_display_off(void);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*SSD1306_H*/
