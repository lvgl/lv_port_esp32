/**
 * @file disp_spi.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"

#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "lvgl/lvgl.h"

#include "disp_spi.h"
#include "disp_driver.h"

#include "../lvgl_driver.h"

/*********************
 *      DEFINES
 *********************/
 #if CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1
 #define TFT_SPI_HOST HSPI_HOST
 #else
 #define TFT_SPI_HOST VSPI_HOST
 #endif

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
static volatile uint8_t spi_pending_trans  = 0;
static transaction_cb_t chained_post_cb;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void disp_spi_add_device_config(spi_host_device_t host, spi_device_interface_config_t *devcfg)
{
    chained_post_cb=devcfg->post_cb;
    devcfg->post_cb=spi_ready;
    esp_err_t ret=spi_bus_add_device(host, devcfg, &spi);
    assert(ret==ESP_OK);
}

void disp_spi_add_device(spi_host_device_t host)
{
    spi_device_interface_config_t devcfg={
#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
        .clock_speed_hz=24*1000*1000,           // Clock out at 24 MHz
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
        .clock_speed_hz=26*1000*1000,           // Clock out at 26 MHz
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
        .clock_speed_hz=8*1000*1000,            // Clock out at 8 MHz
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9486
        .clock_speed_hz=24*1000*1000,           //Clock out at 24 MHz
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_FT81X
			.clock_speed_hz=32*1000*1000,           //Clock out at 32 MHz (30 MHz is spec)
#else
        .clock_speed_hz=40*1000*1000,           // Clock out at 40 MHz
#endif

#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
        .mode=2,                                // SPI mode 2
#else
	    .mode=0,				                // SPI mode 0
#endif
	    .spics_io_num=DISP_SPI_CS,              // CS pin
        .queue_size=1,
        .pre_cb=NULL,
        .post_cb=NULL,
#if !defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_FT81X
        .flags = SPI_DEVICE_HALFDUPLEX
#endif
    };

    disp_spi_add_device_config(host, &devcfg);
}

void disp_spi_init(void)
{
    esp_err_t ret;

    spi_bus_config_t buscfg={
        .mosi_io_num=DISP_SPI_MOSI,
#if CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_FT81X
        .miso_io_num=DISP_SPI_MISO,
#else
        .miso_io_num=-1,
#endif
        .sclk_io_num=DISP_SPI_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
#if defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9341
        .max_transfer_sz = DISP_BUF_SIZE * 2,
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ST7789
        .max_transfer_sz = DISP_BUF_SIZE * 2,
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9488
        .max_transfer_sz = DISP_BUF_SIZE * 3,
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_HX8357
        .max_transfer_sz = DISP_BUF_SIZE * 2
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_ILI9486
            .max_transfer_sz = DISP_BUF_SIZE * 2,
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_SH1107
		.max_transfer_sz = DISP_BUF_SIZE * 2
#elif CONFIG_LVGL_TFT_DISPLAY_CONTROLLER == TFT_CONTROLLER_FT81X
		.max_transfer_sz = DISP_BUF_SIZE * 2
#elif defined CONFIG_LVGL_TFT_DISPLAY_CONTROLLER_IL3820
		.max_transfer_sz = DISP_BUF_SIZE * 2
#endif
    };

    //Initialize the SPI bus
    ret=spi_bus_initialize(TFT_SPI_HOST, &buscfg, 1);
    assert(ret==ESP_OK);

    //Attach the LCD to the SPI bus
    disp_spi_add_device(TFT_SPI_HOST);
}


void disp_spi_transaction(const uint8_t* data, uint16_t length, disp_spi_send_flag_t flags, disp_spi_read_data* out, uint64_t addr)
{
	if (length == 0) return;           //no need to send anything

	// wait for previous pending transaction results
	disp_wait_for_pending_transactions();

	spi_transaction_ext_t t = {0};

	t.base.length = length * 8; // transaction length is in bits

	if (length <= 4 && data != NULL) 
	{
		t.base.flags = SPI_TRANS_USE_TXDATA;
		memcpy(t.base.tx_data, data, length);
	} 
	else 
	{
		t.base.tx_buffer = data;
	}

	if(flags & DISP_SPI_RECEIVE) 
	{
		assert(out !=NULL && (flags & (DISP_SPI_SEND_POLLING | DISP_SPI_SEND_SYNCHRONOUS)));	// sanity check
		t.base.rx_buffer = out;
		t.base.rxlength = 0;	// default to same as tx length
	}

	if(flags & DISP_SPI_ADDRESS_24) 
	{
		t.address_bits = 24;
		t.base.addr = addr;
		t.base.flags |= SPI_TRANS_VARIABLE_ADDR;
	}

	t.base.user = (void*)flags;		// save flags for pre/post transaction processing

	// poll/complete/queue transaction
	if(flags & DISP_SPI_SEND_POLLING) 
	{
		spi_device_polling_transmit(spi, (spi_transaction_t*)&t);
	} 
	else if (flags & DISP_SPI_SEND_SYNCHRONOUS) 
	{
		spi_device_transmit(spi, (spi_transaction_t*)&t);
	} 
	else 
	{
		static spi_transaction_ext_t queuedt;		// must hang around until results retrieved via spi_device_get_trans_result()
		memcpy(&queuedt, &t, sizeof(t));
		spi_pending_trans++;
		if(spi_device_queue_trans(spi, (spi_transaction_t*)&queuedt, portMAX_DELAY) != ESP_OK) 
		{
			spi_pending_trans--;	// clear wait state
		}
	}
}


void disp_wait_for_pending_transactions(void)
{
	spi_transaction_t* presult;
	while(spi_pending_trans)
	{
		if(spi_device_get_trans_result(spi, &presult, portMAX_DELAY) == ESP_OK) 
		{
			spi_pending_trans--;
		}
	}
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

static void IRAM_ATTR spi_ready(spi_transaction_t *trans)
{
	disp_spi_send_flag_t flags = (disp_spi_send_flag_t)trans->user;
	if(flags & DISP_SPI_SIGNAL_FLUSH) 
	{
		lv_disp_t * disp = lv_refr_get_disp_refreshing();
		lv_disp_flush_ready(&disp->driver);
	}

	if(chained_post_cb) chained_post_cb(trans);
}


void disp_spi_acquire()
{
	esp_err_t ret = spi_device_acquire_bus(spi, portMAX_DELAY);
	assert(ret == ESP_OK);
}


void disp_spi_release()
{
	spi_device_release_bus(spi);
}

