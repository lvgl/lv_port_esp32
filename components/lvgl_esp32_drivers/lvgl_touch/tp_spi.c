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
void tp_spi_add_device_config(spi_host_device_t host, spi_device_interface_config_t *devcfg)
{
	esp_err_t ret=spi_bus_add_device(host, devcfg, &spi);
	assert(ret==ESP_OK);
}

void tp_spi_add_device(spi_host_device_t host)
{
	spi_device_interface_config_t devcfg={
		.clock_speed_hz=2*1000*1000,           //Clock out at 2 MHz
		.mode=0,                               //SPI mode 0
		.spics_io_num=TP_SPI_CS,               //CS pin
		.queue_size=1,
		.pre_cb=NULL,
		.post_cb=NULL,
	};
	//Attach the LCD to the SPI bus
	tp_spi_add_device_config(host, &devcfg);
}


void tp_spi_init(void)
{

	esp_err_t ret;

	spi_bus_config_t buscfg={
		.miso_io_num=TP_SPI_MISO,
		.mosi_io_num=TP_SPI_MOSI,
		.sclk_io_num=TP_SPI_CLK,
		.quadwp_io_num=-1,
		.quadhd_io_num=-1
	};

	//Initialize the SPI bus
	ret=spi_bus_initialize(VSPI_HOST, &buscfg, 2);
	assert(ret==ESP_OK);

	//Attach the LCD to the SPI bus
	tp_spi_add_device(VSPI_HOST);
}

void tp_spi_xchg(uint8_t data_send[], uint8_t data_recv[], uint8_t byte_count)
{
	spi_transaction_t t = {
		.length = byte_count * 8, // SPI transaction length is in bits
		.tx_buffer = data_send,
		.rx_buffer = data_recv};

	esp_err_t ret = spi_device_queue_trans(spi, &t, portMAX_DELAY);
	assert(ret == ESP_OK);

	spi_transaction_t *rt;
	ret = spi_device_get_trans_result(spi, &rt, portMAX_DELAY);
	assert(ret == ESP_OK);
	assert(&t == rt);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
