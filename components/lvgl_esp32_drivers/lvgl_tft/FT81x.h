
#ifndef FT81X_H_
#define FT81X_H_

#include <stdint.h>

#include "lvgl/lvgl.h"

#define DISP_BUF_LINES	40
#define DISP_BUF_SIZE   (LV_HOR_RES_MAX * DISP_BUF_LINES)

void FT81x_init(void);

void FT81x_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);

#endif /* FT81X_H_ */
