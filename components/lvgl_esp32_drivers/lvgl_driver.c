/**
 * @file lvgl_driver.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "sdkconfig.h"
#include "lvgl_driver.h"
#include "esp_log.h"

#include "lvgl_tft/disp_spi.h"
#include "lvgl_touch/tp_spi.h"

#include "lvgl_spi_conf.h"

/*********************
 *      DEFINES
 *********************/

 #define TAG "lvgl_driver.c"

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
#ifdef CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI
#ifdef SHARED_SPI_BUS
static void configure_shared_spi_bus(void);
#endif  // SHARED_SPI_BUS
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void lvgl_driver_init(void)
{
    /* Interface and driver initialization */
#ifdef CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI
#ifdef SHARED_SPI_BUS
	/* Configure one SPI bus for the two devices */
	configure_shared_spi_bus();

	/* Configure the drivers */
	disp_driver_init(false);
#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	touch_driver_init(false);
#endif // CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
#else
	/* Otherwise configure the SPI bus and devices separately inside the drivers*/
	disp_driver_init(true);
#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
	touch_driver_init(true);
#endif  // CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
#endif  // SHARED_SPI_BUS
#endif  // CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI

#if defined CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_I2C
	disp_driver_init(false);
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#ifdef CONFIG_LVGL_TFT_DISPLAY_PROTOCOL_SPI
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
                .max_transfer_sz = SPI_BUS_MAX_TRANSFER_SZ
	};

	esp_err_t ret = spi_bus_initialize(TFT_SPI_HOST, &buscfg, 1);
	assert(ret == ESP_OK);

	/* SPI Devices */
	disp_spi_add_device(TFT_SPI_HOST);
	tp_spi_add_device(TOUCH_SPI_HOST);
}
#endif  // SHARED_SPI_BUS
#endif
