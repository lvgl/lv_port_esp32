/**
 * @file tp_spi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "tp_spi.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include <string.h>

/*********************
 *      DEFINES
 *********************/
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1
#define TOUCH_SPI_HOST HSPI_HOST
#else
#define TOUCH_SPI_HOST VSPI_HOST
#endif

// Only initialize a SPI bus if both devices don't use the same SPI bus,
// otherwise the disp_spi module will take care of it for us
#if CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_VSPI == 1
#define INIT_SPI_BUS
#endif
#else
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1
#define INIT_SPI_BUS
#endif
#endif

/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  STATIC PROTOTYPES
 **********************/
static spi_device_handle_t spi;

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void tp_spi_init(void)
{

	esp_err_t ret;

#ifdef INIT_SPI_BUS
	spi_bus_config_t buscfg={
		.miso_io_num=TP_SPI_MISO,
		.mosi_io_num=TP_SPI_MOSI,
		.sclk_io_num=TP_SPI_CLK,
		.quadwp_io_num=-1,
		.quadhd_io_num=-1
	};
#endif

	spi_device_interface_config_t devcfg={
#if CONFIG_LVGL_TOUCH_CONTROLLER == 3
		.clock_speed_hz=1*1000*1000,           //Clock out at 1 MHz
#else
		.clock_speed_hz=2*1000*1000,           //Clock out at 2 MHz
#endif
#if CONFIG_LVGL_TOUCH_CONTROLLER == 3
		.mode=1,								//SPI mode 1
#else
		.mode=0,                                //SPI mode 0
#endif
		.spics_io_num=-1,              //CS pin
		.queue_size=1,
		.pre_cb=NULL,
		.post_cb=NULL,
	};

#ifdef INIT_SPI_BUS
	//Initialize the SPI bus
	ret=spi_bus_initialize(TOUCH_SPI_HOST, &buscfg, 2);
	assert(ret==ESP_OK);
#endif

	//Attach the LCD to the SPI bus
	ret=spi_bus_add_device(TOUCH_SPI_HOST, &devcfg, &spi);
	assert(ret==ESP_OK);
}

uint8_t tp_spi_xchg(uint8_t data_send)
{
    uint8_t data_recv = 0;
    
    spi_transaction_t t = {
        .length = 8, // length is in bits
        .tx_buffer = &data_send,
        .rx_buffer = &data_recv
    };

    spi_device_queue_trans(spi, &t, portMAX_DELAY);

    spi_transaction_t * rt;
    spi_device_get_trans_result(spi, &rt, portMAX_DELAY);

    return data_recv;
}


/**********************
 *   STATIC FUNCTIONS
 **********************/
