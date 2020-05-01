/**
 * @file disp_i2c.c
 */

/*********************
 *      INCLUDES
 *********************/
#include <string.h>

#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "lvgl/lvgl.h"

#include "disp_i2c.h"
#include "disp_driver.h"

#include "../lvgl_driver.h"
#include "../lvgl_i2c_conf.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void i2c_master_init(void)
{
    i2c_config_t i2c_config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SSD_1306_SDA,
        .scl_io_num = SSD_1306_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = OLED_IIC_FREQ_HZ
    };

    i2c_param_config(I2C_NUM_0, &i2c_config);
    i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

