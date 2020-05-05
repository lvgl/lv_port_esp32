/**
* @file ADCRAW.h
*/

#ifndef ADCRAW_H
#define ADCRAW_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "driver/gpio.h"
#include "driver/adc.h"
#include "lvgl/lvgl.h"

#define TOUCHSCREEN_RESISTIVE_PIN_YU 26 // Y+ any gpio
#define TOUCHSCREEN_RESISTIVE_PIN_YD 32 // Y- also ADC
#define TOUCHSCREEN_RESISTIVE_PIN_XL 27 // X- any gpio
#define TOUCHSCREEN_RESISTIVE_PIN_XR 33 // X+ also ADC

// Default calibration points
#define TOUCHCAL_ULX 29  // Upper Left X
#define TOUCHCAL_ULY 84  // Upper Left Y
#define TOUCHCAL_URX 828 // Upper Right X
#define TOUCHCAL_URY 60  // Upper Right Y
#define TOUCHCAL_LLX 29  // Lower Left X
#define TOUCHCAL_LLY 928 // Lower Left Y
#define TOUCHCAL_LRX 828 // Lower Right X
#define TOUCHCAL_LRY 928 // Lower Right Y

#define TOUCHSCREEN_RESISTIVE_PRESS_THRESHOLD 1023
	
/*GetMaxX Macro*/
#if CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE
#define GetMaxX()   (CONFIG_LVGL_DISPLAY_WIDTH - 1)
#else
#define GetMaxX()   (CONFIG_LVGL_DISPLAY_HEIGHT - 1)
#endif

/*GetMaxY Macro*/
#if CONFIG_LVGL_DISPLAY_ORIENTATION_LANDSCAPE
#define GetMaxY()   (CONFIG_LVGL_DISPLAY_HEIGHT - 1)
#else
#define GetMaxY()   (CONFIG_LVGL_DISPLAY_WIDTH - 1)
#endif

#ifndef CONCAT3
#define _CONCAT3(a,b,c) a ## b ## c
#define CONCAT3(a,b,c) _CONCAT3(a,b,c)
#endif

#define GPIO_TO_ADC_ELEMENT(x) [x] = CONCAT3(ADC1_GPIO, x, _CHANNEL)

#define ADC1_GPIO36_CHANNEL     ADC1_CHANNEL_0
#define ADC1_CHANNEL_0_GPIO_NUM 36

#define ADC1_GPIO37_CHANNEL     ADC1_CHANNEL_1
#define ADC1_CHANNEL_1_GPIO_NUM 37

#define ADC1_GPIO38_CHANNEL     ADC1_CHANNEL_2
#define ADC1_CHANNEL_2_GPIO_NUM 38

#define ADC1_GPIO39_CHANNEL     ADC1_CHANNEL_3
#define ADC1_CHANNEL_3_GPIO_NUM 39

#define ADC1_GPIO32_CHANNEL     ADC1_CHANNEL_4
#define ADC1_CHANNEL_4_GPIO_NUM 32

#define ADC1_GPIO33_CHANNEL     ADC1_CHANNEL_5
#define ADC1_CHANNEL_5_GPIO_NUM 33

#define ADC1_GPIO34_CHANNEL     ADC1_CHANNEL_6
#define ADC1_CHANNEL_6_GPIO_NUM 34

#define ADC1_GPIO35_CHANNEL     ADC1_CHANNEL_7
#define ADC1_CHANNEL_7_GPIO_NUM 35

typedef enum {
    IDLE,
    SET_X,
    SET_Y,
    SET_Z1,
    SET_Z2,
	READ_X,
	READ_Y,
	READ_Z1,
	READ_Z2
} TOUCH_STATES; 

void adcraw_init(void);
bool adcraw_read(lv_indev_drv_t * drv, lv_indev_data_t * data);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ADCRAW_H */
