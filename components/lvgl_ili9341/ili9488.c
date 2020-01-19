/**
 * @file ili9488.c
 */

/*********************
 *      INCLUDES
 *********************/
#include "ili9488.h"
#include "disp_spi.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/*********************
 *      DEFINES
 *********************/

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
static void ili9488_send_cmd(uint8_t cmd);
static void ili9488_send_data(void * data, uint16_t length);
static void ili9488_send_color(void * data, uint16_t length);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// From github.com/jeremyjh/ESP32_TFT_library 
void ili9488_init(void)
{
	lcd_init_cmd_t ili_init_cmds[]={
		{0xE0, {0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C,
		0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F}, 15},   /*Positive gamma control*/
		{0xE1, {0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46,
		0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F}, 15},   /*Negative gamma control*/
		{0xC0, {0x17, 0x15}, 2},	/*Power control 1*/
		{0xC1, {0x41}, 1},		/*Power control 2*/
		{0xC5, {0x00, 0x12, 0x80}, 3},	/*Power control 3*/
		{0x36, {0x48}, 1},		/*Memory Access Control*/
		{0x3A, {0x66}, 1},		/*Pixel Format Set*/
		{0xB0, {0x00}, 1},		/*Interface mode control*/
		{0xB1, {0xA0}, 1},		/*Frame rate, 0xA0 = 60Hz*/
		{0xB4, {0x02}, 1},		/*Display inversion control*/
		{0xB6, {0x02, 0x02, 0x3B}, 3},	/*Display function control RGB/MCU interface control*/
		{0xB7, {0xC6}, 1},
		// {0xE9, {0x00}, 1},		/*Set image function, disable 24bit data*/
		// {0x53, {0x28}, 1},		/*Write CTRL display value*/
		// {0x51, {0x7F}, 1},		/*Write display brightness value*/
		{0xF7, {0xA9, 0x51, 0x2C, 0x82}, 4}, /*Adjust control*/
		{0, {0}, 0xff},
	};

	//Initialize non-SPI GPIOs
	gpio_set_direction(ILI9488_DC, GPIO_MODE_OUTPUT);
	gpio_set_direction(ILI9488_RST, GPIO_MODE_OUTPUT);
	gpio_set_direction(ILI9488_BCKL, GPIO_MODE_OUTPUT);

	//Reset the display
	gpio_set_level(ILI9488_RST, 0);
	vTaskDelay(100 / portTICK_RATE_MS);
	gpio_set_level(ILI9488_RST, 1);
	vTaskDelay(100 / portTICK_RATE_MS);

	printf("ILI9488 initialization.\n");

	//Send all the commands
	uint16_t cmd = 0;
	while (ili_init_cmds[cmd].databytes!=0xff) {
		ili9488_send_cmd(ili_init_cmds[cmd].cmd);
		ili9488_send_data(ili_init_cmds[cmd].data, ili_init_cmds[cmd].databytes&0x1F);
		if (ili_init_cmds[cmd].databytes & 0x80) {
			vTaskDelay(100 / portTICK_RATE_MS);
		}
		cmd++;
	}

	// Exit sleep
	ili9488_send_cmd(0x11);
	vTaskDelay(120 / portTICK_RATE_MS);
	// Display on
	ili9488_send_cmd(0x29);
	vTaskDelay(25 / portTICK_RATE_MS);

	ili9488_enable_backlight(true);

#if ILI9488_INVERT_DISPLAY
	uint8_t data[] = {0x68};
	// this same command also sets rotation (portrait/landscape) and inverts colors.
	// https://gist.github.com/motters/38a26a66020f674b6389063932048e4c#file-ili9844_defines-h-L24
	ili9488_send_cmd(0x36);
	ili9488_send_data(&data, 1);
#endif
}


void ili9488_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{

	uint8_t data[4];

	/*Column addresses*/
	ili9488_send_cmd(0x2A);
	data[0] = (area->x1 >> 8) & 0xFF;
	data[1] = area->x1 & 0xFF;
	data[2] = (area->x2 >> 8) & 0xFF;
	data[3] = area->x2 & 0xFF;
	ili9488_send_data(data, 4);

	/*Page addresses*/
	ili9488_send_cmd(0x2B);
	data[0] = (area->y1 >> 8) & 0xFF;
	data[1] = area->y1 & 0xFF;
	data[2] = (area->y2 >> 8) & 0xFF;
	data[3] = area->y2 & 0xFF;
	ili9488_send_data(data, 4);

	/*Memory write*/
	ili9488_send_cmd(0x2C);

	uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

	ili9488_send_color((void*)color_map, size * 2);
}

void ili9488_enable_backlight(bool backlight)
{
#if ILI9488_ENABLE_BACKLIGHT_CONTROL
    printf("%s backlight.\n", backlight ? "Enabling" : "Disabling");
    uint32_t tmp = 0;

#if (ILI9488_BCKL_ACTIVE_LVL==1)
    tmp = backlight ? 1 : 0;
#else
    tmp = backlight ? 0 : 1;
#endif

    gpio_set_level(ILI9488_BCKL, tmp);
#endif
}

/**********************
 *   STATIC FUNCTIONS
 **********************/


static void ili9488_send_cmd(uint8_t cmd)
{
	  while(disp_spi_is_busy()) {}
	  gpio_set_level(ILI9488_DC, 0);	 /*Command mode*/
	  disp_spi_send_data(&cmd, 1);
}

static void ili9488_send_data(void * data, uint16_t length)
{
	  while(disp_spi_is_busy()) {}
	  gpio_set_level(ILI9488_DC, 1);	 /*Data mode*/
	  disp_spi_send_data(data, length);
}

static void ili9488_send_color(void * data, uint16_t length)
{
		while(disp_spi_is_busy()) {}
    gpio_set_level(ILI9488_DC, 1);   /*Data mode*/
    disp_spi_send_colors(data, length);
}

