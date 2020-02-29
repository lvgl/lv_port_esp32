/* LVGL Example project
 * 
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "lvgl/lvgl.h"
#include "lv_examples/lv_apps/demo/demo.h"
#include "esp_freertos_hooks.h"


#include "disp_spi.h"
#include "disp_driver.h"

#include "tp_spi.h"
#include "xpt2046.h"
#include "tp_i2c.h"
#include "ft6x36.h"

static void IRAM_ATTR lv_tick_task(void);

#if CONFIG_LVGL_TOUCH_CONTROLLER == 1 && TP_SPI_MOSI == DISP_SPI_MOSI && TP_SPI_CLK == DISP_SPI_CLK
/* Example function that configure two spi devices (tft and touch controllers) into the same spi bus */
static void configure_shared_spi_bus(void);
#endif

void app_main() {
    lv_init();

    /* Configure the SPI bus and SPI devices to handle the tft and touch controllers */
#if CONFIG_LVGL_TOUCH_CONTROLLER == 1 && TP_SPI_MOSI == DISP_SPI_MOSI && TP_SPI_CLK == DISP_SPI_CLK
    configure_shared_spi_bus();
#else
    /* Otherwise configure the SPI bus and devices separately */
    disp_spi_init();
    disp_driver_init();

#if CONFIG_LVGL_TOUCH_CONTROLLER == 1
    tp_spi_init();
    xpt2046_init();
#elif CONFIG_LVGL_TOUCH_CONTROLLER == 2
    ft6x06_init(FT6236_I2C_SLAVE_ADDR);
#endif
#endif

    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

#if CONFIG_LVGL_TOUCH_CONTROLLER == 1
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = xpt2046_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#elif CONFIG_LVGL_TOUCH_CONTROLLER == 2
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = ft6x36_touch_xy;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

    esp_register_freertos_tick_hook(lv_tick_task);

    demo_create();

    while (1) {
        vTaskDelay(1);
        lv_task_handler();
    }
}

static void IRAM_ATTR lv_tick_task(void) {
    lv_tick_inc(portTICK_RATE_MS);
}

#if CONFIG_LVGL_TOUCH_CONTROLLER == 1 && TP_SPI_MOSI == DISP_SPI_MOSI && TP_SPI_CLK == DISP_SPI_CLK
static void configure_shared_spi_bus(void)
{
    spi_bus_config_t buscfg = {
        .miso_io_num = TP_SPI_MISO,
        .mosi_io_num = TP_SPI_MOSI,
        .sclk_io_num = TP_SPI_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = DISP_BUF_SIZE * 2,
    };

    esp_err_t ret = spi_bus_initialize(HSPI_HOST, &buscfg, 1);
    assert(ret == ESP_OK);

    spi_device_interface_config_t ili9341_config = {
        .clock_speed_hz = 20 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = DISP_SPI_CS,
        .queue_size = 1,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    disp_spi_add_device_config(HSPI_HOST, &ili9341_config);
    disp_driver_init();

    spi_device_interface_config_t xpt2046_config = {
        .clock_speed_hz = 2 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = TP_SPI_CS,
        .queue_size = 1,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    tp_spi_add_device_config(HSPI_HOST, &xpt2046_config);
    xpt2046_init();
}
#endif
