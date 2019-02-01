/**
 * @file disp_spi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "disp_spi.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "../lvgl/lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void IRAM_ATTR spi_ready (spi_transaction_t *trans);

/**********************
 *  STATIC VARIABLES
 **********************/
static spi_device_handle_t spi;
static SemaphoreHandle_t xSemaphore;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void disp_spi_init(void)
{

	esp_err_t ret;

	spi_bus_config_t buscfg={
		.miso_io_num=-1,
		.mosi_io_num=DISP_SPI_MOSI,
		.sclk_io_num=DISP_SPI_CLK,
		.quadwp_io_num=-1,
		.quadhd_io_num=-1,
		.max_transfer_sz = LV_VDB_SIZE * 2,
	};

	spi_device_interface_config_t devcfg={
		.clock_speed_hz=40*1000*1000,           //Clock out at 80 MHz
		.mode=0,                                //SPI mode 0
		.spics_io_num=DISP_SPI_CS,              //CS pin
		.queue_size=1,
		.pre_cb=NULL,
		.post_cb=spi_ready,
	};

	//Initialize the SPI bus
	ret=spi_bus_initialize(HSPI_HOST, &buscfg, 1);
	assert(ret==ESP_OK);

	//Attach the LCD to the SPI bus
	ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
	assert(ret==ESP_OK);

	xSemaphore = xSemaphoreCreateBinary();


}

void disp_spi_send(uint8_t * data, uint16_t length)
{
	if (length == 0) return;           //no need to send anything

	xSemaphoreTake(xSemaphore, portMAX_DELAY);

    printf("sending\n");
	spi_transaction_t t;
    memset(&t, 0, sizeof(t));       	//Zero out the transaction
	t.length = length * 8;              //Length is in bytes, transaction length is in bits.
	t.tx_buffer = data;               	//Data

	spi_device_queue_trans(spi, &t, portMAX_DELAY);
    printf("queued\n");
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void IRAM_ATTR spi_ready (spi_transaction_t *trans)
{
    printf("send ready\n");
    spi_transaction_t * rt;
    spi_device_get_trans_result(spi, &rt, portMAX_DELAY);

    lv_flush_ready();
    BaseType_t b = pdTRUE;
    xSemaphoreGiveFromISR(xSemaphore, &b);
}
