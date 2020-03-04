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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"

#include "esp_system.h"
#include "driver/gpio.h"

/* Littlevgl specific */
#include "lvgl/lvgl.h"
#include "lv_examples/lv_apps/demo/demo.h"

#include "disp_spi.h"
#include "disp_driver.h"
#include "tp_spi.h"
#include "touch_driver.h"


/*********************
 *      DEFINES
 *********************/
// Detect the use of a shared SPI Bus and verify the user specified the same SPI bus for both touch and tft
#if (CONFIG_LVGL_TOUCH_CONTROLLER == 1 || CONFIG_LVGL_TOUCH_CONTROLLER == 3) && TP_SPI_MOSI == DISP_SPI_MOSI && TP_SPI_CLK == DISP_SPI_CLK
#if CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1
#define TFT_SPI_HOST HSPI_HOST
#else
#define TFT_SPI_HOST VSPI_HOST
#endif
 
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1
#define TOUCH_SPI_HOST HSPI_HOST
#else
#define TOUCH_SPI_HOST VSPI_HOST
#endif

#if TFT_SPI_HOST != TOUCH_SPI_HOST
#error You must specifiy the same SPI host for both display and input driver
#endif

#define SHARED_SPI_BUS
#endif

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void IRAM_ATTR lv_tick_task(void);

#ifdef SHARED_SPI_BUS
/* Example function that configure two spi devices (tft and touch controllers) into the same spi bus */
static void configure_shared_spi_bus(void);
#endif


/**********************
 *   APPLICATION MAIN
 **********************/
void app_main() {
	lv_init();

	/* Interface and driver initialization */
#ifdef SHARED_SPI_BUS
	/* Configure one SPI bus for the two devices */
	configure_shared_spi_bus();
    
	/* Configure the drivers */
	disp_driver_init(false);
#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	touch_driver_init(false);
#endif
#else
	/* Otherwise configure the SPI bus and devices separately inside the drivers*/
	disp_driver_init(true);
#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	touch_driver_init(true);
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

#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	lv_indev_drv_t indev_drv;
	lv_indev_drv_init(&indev_drv);
	indev_drv.read_cb = touch_driver_read;
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

#ifdef SHARED_SPI_BUS
static void configure_shared_spi_bus(void)
{
	/* Shared SPI bus configuration */
	spi_bus_config_t buscfg = {
		.miso_io_num = TP_SPI_MISO,
		.mosi_io_num = DISP_SPI_MOSI,
		.sclk_io_num = DISP_SPI_CLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9341
		.max_transfer_sz = DISP_BUF_SIZE * 2,
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ST7789
		.max_transfer_sz = DISP_BUF_SIZE * 2,
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_ILI9488
		.max_transfer_sz = DISP_BUF_SIZE * 3,
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_HX8357
		.max_transfer_sz = DISP_BUF_SIZE * 2
#endif
	};

	esp_err_t ret = spi_bus_initialize(TFT_SPI_HOST, &buscfg, 1);
	assert(ret == ESP_OK);

	/* SPI Devices */
	disp_spi_add_device(TFT_SPI_HOST);
	tp_spi_add_device(TOUCH_SPI_HOST);
}
#endif
