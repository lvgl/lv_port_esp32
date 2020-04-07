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
	lcd_init_cmd_t init_cmds[]={
        {0xAE, {0}, 0},         // Turn display off
        {0xDC, {0x00}, 1},      // Set display start line
        {0x81, {0x2F}, 1},       // Set display contrast
        {0x20, {0}, 0},         // Set memory mode
        // {0x21, {0}, 0},         // Set memory mode
        {0xA0, {0}, 0},         // Non-rotated display
        {0xC0, {0}, 0},         // Non-flipped vertical
        {0xA8, {0x7F}, 1},      // Set multiplex ratio
        {0xD3, {0x60}, 1},      // Set display offset to zero
        {0xD5, {0x51}, 1},      // Set display clock divider
        {0xD9, {0x22}, 1},      // Set pre-charge
        {0xDB, {0x35}, 1},      // Set com detect
        {0xB0, {0}, 0},         // Set page address
        {0xDA, {0x12}, 1},      // Set com pins
        {0xA4, {0}, 0},         // output ram to display
        {0xA6, {0}, 0},         // Non-inverted display
        // {0xA7, {0}, 0},         // Inverted display     // not included
        {0xAF,{0}, 0},          // Turn display on
		{0, {0}, 0xff},
	};

    ESP_LOGI(TAG, "sh1107_init called");

	//Initialize non-SPI GPIOs
	gpio_set_direction(SH1107_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(SH1107_RST, GPIO_MODE_OUTPUT);

	//Reset the display
	gpio_set_level(SH1107_RST, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	gpio_set_level(SH1107_RST, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	ESP_LOGI(TAG, "SH1107 initialization.");

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
    uint16_t byte_index = x + (( y>>3 ) * buf_w);
    uint8_t  bit_index  = y & 0x7;

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
    uint8_t row1 = area->y1>>3;
    uint8_t row2 = area->y2>>3;

	ESP_LOGI(TAG, "sh1107_flush page = %x columLow = %x columHigh = %x", row1, columnLow, columnHigh);

	sh1107_send_cmd(0x10 | columnLow);  // Set Higher Column Start Address for Page Addressing Mode
	sh1107_send_cmd(0x00 | columnHigh); // Set Lower Column Start Address for Page Addressing Mode
	sh1107_send_cmd(0xB0 | row1);       // Set Page Start Address for Page Addressing Mode

	uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);
	sh1107_send_color((void*)color_map, size);
}

void sh1107_rounder(struct _disp_drv_t * disp_drv, lv_area_t *area)
{
    // ESP_LOGI(TAG, "sh1107_rounder > (%d %d) -> (%d %d)", area->x1, area->y1, area->x2,  area->y2);
    area->y1 = (area->y1 & (~0x7));
    area->y2 = (area->y2 & (~0x7)) + 7;
    // ESP_LOGI(TAG, "sh1107_rounder < (%d %d) -> (%d %d)", area->x1, area->y1, area->x2,  area->y2);
}

void sh1107_sleep_in()
{
	// uint8_t data[] = {0x08};
	// sh1107_send_cmd(0x10);
	// sh1107_send_data(&data, 1);
}

void sh1107_sleep_out()
{
	// uint8_t data[] = {0x08};
	// sh1107_send_cmd(0x11);
	// sh1107_send_data(&data, 1);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void sh1107_send_cmd(uint8_t cmd)
{
	// ESP_LOGI(TAG, "sh1107_send_cmd %x", cmd);
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 0);	 /*Command mode*/
    disp_spi_send_data(&cmd, 1);
}

static void sh1107_send_data(void * data, uint16_t length)
{
	// ESP_LOGI(TAG, "sh1107_send_data length=%d", length);
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 1);	 /*Data mode*/
    disp_spi_send_data(data, length);
}

static void sh1107_send_color(void * data, uint16_t length)
{
	// ESP_LOGI(TAG, "sh1107_send_color length=%d", length);
    while(disp_spi_is_busy()) {}
    gpio_set_level(SH1107_DC, 1);   /*Data mode*/
    disp_spi_send_colors(data, length);
}

