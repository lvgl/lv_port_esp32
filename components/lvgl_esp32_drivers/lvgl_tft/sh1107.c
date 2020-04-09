/**
 * @file sh1107.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "sh1107.h"
#include "disp_spi.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*********************
 *      DEFINES
 *********************/
 #define TAG "SH1107"

/**********************
 *      TYPEDEFS
 **********************/

/*The LCD needs a bunch of command/argument values to be initialized. They are stored in this struct. */
typedef struct {
    uint8_t cmd;
    uint8_t data[16];
    uint8_t databytes; //No of data in data; bit 7 = delay after set; 0xFF = end of cmds.
} lcd_init_cmd_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void sh1107_send_cmd(uint8_t cmd);
static void sh1107_send_data(void * data, uint16_t length);
static void sh1107_send_color(void * data, uint16_t length);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

#define BIT_SET(a,b) ((a) |= (1U<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1U<<(b)))

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void sh1107_init(void)
{
    // Use Double Bytes Commands if necessary, but not Command+Data
    // Initialization taken from https://github.com/nopnop2002/esp-idf-m5stick
	lcd_init_cmd_t init_cmds[]={
    	{0xAE, {0}, 0},	// Turn display off
    	{0xDC, {0}, 0},	// Set display start line
    	{0x00, {0}, 0},	// ...value
    	{0x81, {0}, 0},	// Set display contrast
    	{0x2F, {0}, 0},	// ...value
    	{0x20, {0}, 0},	// Set memory mode
    	{0xA0, {0}, 0},	// Non-rotated display  
    	{0xC8, {0}, 0},	// flipped vertical
    	{0xA8, {0}, 0},	// Set multiplex ratio
    	{0x7F, {0}, 0},	// ...value
    	{0xD3, {0}, 0},	// Set display offset to zero
    	{0x60, {0}, 0},	// ...value
    	{0xD5, {0}, 0},	// Set display clock divider
    	{0x51, {0}, 0},	// ...value
    	{0xD9, {0}, 0},	// Set pre-charge
    	{0x22, {0}, 0},	// ...value
    	{0xDB, {0}, 0},	// Set com detect
    	{0x35, {0}, 0},	// ...value
    	{0xB0, {0}, 0},	// Set page address
    	{0xDA, {0}, 0},	// Set com pins
    	{0x12, {0}, 0},	// ...value
    	{0xA4, {0}, 0},	// output ram to display
    	{0xA6, {0}, 0},	// Non-inverted display
    	{0xAF, {0}, 0},	// Turn display on
        {0, {0}, 0xff},
	};

	//Initialize non-SPI GPIOs
	gpio_set_direction(SH1107_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(SH1107_RST, GPIO_MODE_OUTPUT);

	//Reset the display
	gpio_set_level(SH1107_RST, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	gpio_set_level(SH1107_RST, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	//Send all the commands
	uint16_t cmd = 0;
	while (init_cmds[cmd].databytes!=0xff) {
		sh1107_send_cmd(init_cmds[cmd].cmd);
		sh1107_send_data(init_cmds[cmd].data, init_cmds[cmd].databytes&0x1F);
		if (init_cmds[cmd].databytes & 0x80) {
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		cmd++;
	}
}

void sh1107_set_px_cb(struct _disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
        lv_color_t color, lv_opa_t opa) {
    uint16_t byte_index = y + (( x>>3 ) * 64);  // TODO
    uint8_t  bit_index  = x & 0x7;

    if ( color.full == 0 ) {
        BIT_SET(buf[byte_index], bit_index);
    } else {
        BIT_CLEAR(buf[byte_index], bit_index);
    }
}

void sh1107_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    uint8_t columnLow = area->x1 & 0x0F;
	uint8_t columnHigh = (area->x1 >> 4) & 0x0F;
    uint8_t row1 = area->x1>>3;
    uint8_t row2 = area->x2>>3;
    uint32_t size = 0;
    void *ptr;

    for(int i = row1; i < row2+1; i++){
	    sh1107_send_cmd(0x10 | columnHigh);         // Set Higher Column Start Address for Page Addressing Mode
	    sh1107_send_cmd(0x00 | columnLow);          // Set Lower Column Start Address for Page Addressing Mode
	    sh1107_send_cmd(0xB0 | i);                  // Set Page Start Address for Page Addressing Mode
	    size = area->y2 - area->y1 + 1;
        ptr = color_map + i * 64;                   // TODO width/height of display
        sh1107_send_color( (void *) ptr, size);
    }
}

void sh1107_rounder(struct _disp_drv_t * disp_drv, lv_area_t *area)
{
    area->y1 = (area->y1 & (~0x7));
    area->y2 = (area->y2 & (~0x7)) + 7;
}

void sh1107_sleep_in()
{
	sh1107_send_cmd(0xAE);
}

void sh1107_sleep_out()
{
	sh1107_send_cmd(0xAF);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void sh1107_send_cmd(uint8_t cmd)
{
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 0);	 /*Command mode*/
    disp_spi_send_data(&cmd, 1);
}

static void sh1107_send_data(void * data, uint16_t length)
{
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 1);	 /*Data mode*/
    disp_spi_send_data(data, length);
}

static void sh1107_send_color(void * data, uint16_t length)
{
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 1);   /*Data mode*/
    disp_spi_send_colors(data, length);
}
