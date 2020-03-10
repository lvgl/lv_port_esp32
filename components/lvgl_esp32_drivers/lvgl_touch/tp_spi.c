/**
 * @file tp_spi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "tp_spi.h"
#include "touch_driver.h"
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
#if CONFIG_LVGL_TOUCH_CONTROLLER == TOUCH_CONTROLLER_STMPE610
		.clock_speed_hz=1*1000*1000,           //Clock out at 1 MHz
		.mode=1,                               //SPI mode 1
#else
		.clock_speed_hz=2*1000*1000,           //Clock out at 2 MHz
		.mode=0,                               //SPI mode 0
#endif
		.spics_io_num=TP_SPI_CS,               //CS pin
		.queue_size=1,
		.pre_cb=NULL,
		.post_cb=NULL,
		.command_bits = 8,
		.address_bits = 0,
		.dummy_bits = 0,
		.flags = SPI_DEVICE_HALFDUPLEX | SPI_DEVICE_NO_DUMMY,
	};
	
	//Attach the Touch controller to the SPI bus
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
	ret=spi_bus_initialize(TOUCH_SPI_HOST, &buscfg, 2);
	assert(ret==ESP_OK);

	//Attach the Touch controller to the SPI bus
	tp_spi_add_device(TOUCH_SPI_HOST);
}

void tp_spi_xchg(uint8_t* data_send, uint8_t* data_recv, uint8_t byte_count)
{
	spi_transaction_t t = {
		.length = byte_count * 8, // SPI transaction length is in bits
		.tx_buffer = data_send,
		.rx_buffer = data_recv};
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert(ret == ESP_OK);
}

void tp_spi_write_reg(uint8_t* data, uint8_t byte_count)
{
	spi_transaction_t t = {
	    .length = byte_count * 8,
	    .tx_buffer = data,
	    .flags = 0
	};
	
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert(ret == ESP_OK);
}

void tp_spi_read_reg(uint8_t reg, uint8_t* data, uint8_t byte_count)
{
	spi_transaction_t t = {	
	    .length = byte_count * 8 + sizeof(reg),
	    .rxlength = byte_count * 8,
	    .cmd = reg,
	    .rx_buffer = data,
	    .flags = 0
	};
	
	// Read - send first byte as command
	esp_err_t ret = spi_device_transmit(spi, &t);
	assert(ret == ESP_OK);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
