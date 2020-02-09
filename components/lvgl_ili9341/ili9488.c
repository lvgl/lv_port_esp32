/**
 * @file ili9488.c
 */

/*********************
 *      INCLUDES
 *********************/
#include "ili9488.h"
#include "disp_spi.h"
#include "driver/gpio.h"
#include "esp_heap_caps.h"

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
// From github.com/mvturnho/ILI9488-lvgl-ESP32-WROVER-B
void ili9488_init(void)
{
	lcd_init_cmd_t ili_init_cmds[]={
                {ILI9488_CMD_SLEEP_OUT, {0x00}, 0x80},
		{ILI9488_CMD_POSITIVE_GAMMA_CORRECTION, {0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F}, 15},
		{ILI9488_CMD_NEGATIVE_GAMMA_CORRECTION, {0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F}, 15},
		{ILI9488_CMD_POWER_CONTROL_1, {0x17, 0x15}, 2},
		{ILI9488_CMD_POWER_CONTROL_2, {0x41}, 1},
		{ILI9488_CMD_VCOM_CONTROL_1, {0x00, 0x12, 0x80}, 3},
		{ILI9488_CMD_MEMORY_ACCESS_CONTROL, {(0x20 | 0x08)}, 1},
		{ILI9488_CMD_COLMOD_PIXEL_FORMAT_SET, {0x66}, 1},
		{ILI9488_CMD_INTERFACE_MODE_CONTROL, {0x00}, 1},
		{ILI9488_CMD_FRAME_RATE_CONTROL_NORMAL, {0xA0}, 1},
		{ILI9488_CMD_DISPLAY_INVERSION_CONTROL, {0x02}, 1},
		{ILI9488_CMD_DISPLAY_FUNCTION_CONTROL, {0x02, 0x02}, 2},
		{ILI9488_CMD_SET_IMAGE_FUNCTION, {0x00}, 1},
		{ILI9488_CMD_WRITE_CTRL_DISPLAY, {0x28}, 1},
		{ILI9488_CMD_WRITE_DISPLAY_BRIGHTNESS, {0x7F}, 1},
		{ILI9488_CMD_ADJUST_CONTROL_3, {0xA9, 0x51, 0x2C, 0x02}, 4},
		{ILI9488_CMD_DISPLAY_ON, {0x00}, 0x80},
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

	// Exit sleep
	ili9488_send_cmd(0x01);	/* Software reset */
	vTaskDelay(100 / portTICK_RATE_MS);
	
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

	ili9488_enable_backlight(true);

#if ILI9488_INVERT_DISPLAY
	uint8_t data[] = {0x68};
	// this same command also sets rotation (portrait/landscape) and inverts colors.
	// https://gist.github.com/motters/38a26a66020f674b6389063932048e4c#file-ili9844_defines-h-L24
	ili9488_send_cmd(0x36);
	ili9488_send_data(&data, 1);
#endif
}

// Flush function based on mvturnho repo
void ili9488_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

    lv_color16_t *buffer_16bit = (lv_color16_t *) color_map;
    uint8_t *mybuf = (uint8_t *) heap_caps_malloc(3 * size * sizeof(uint8_t), MALLOC_CAP_DMA);

    uint32_t LD = 0;
    uint32_t j = 0;

    for (uint32_t i = 0; i < size; i++) {
        LD = buffer_16bit[i].full;
        mybuf[j] = (uint8_t) ((LD & 0xF800) >> 8);
        j++;
        mybuf[j] = (uint8_t) ((LD & 0x07E0) >> 3);
        j++;
        mybuf[j] = (uint8_t) ((LD & 0x001F) << 3);
        j++;
    }

	/* Column addresses  */
	uint8_t xb[] = {
	    (uint8_t) (area->x1 >> 8) & 0xFF,
	    (uint8_t) (area->x1) & 0xFF,
	    (uint8_t) (area->x2 >> 8) & 0xFF,
	    (uint8_t) (area->x2) & 0xFF,
	};
	
	/* Page addresses  */
	uint8_t yb[] = {
	    (uint8_t) (area->y1 >> 8) & 0xFF,
	    (uint8_t) (area->y1) & 0xFF,
	    (uint8_t) (area->y2 >> 8) & 0xFF,
	    (uint8_t) (area->y2) & 0xFF,
	};

	/*Column addresses*/
	ili9488_send_cmd(ILI9488_CMD_COLUMN_ADDRESS_SET);
	ili9488_send_data(xb, 4);

	/*Page addresses*/
	ili9488_send_cmd(ILI9488_CMD_PAGE_ADDRESS_SET);
	ili9488_send_data(yb, 4);

	/*Memory write*/
	ili9488_send_cmd(ILI9488_CMD_MEMORY_WRITE);

	ili9488_send_color((void *) mybuf, size * 3);
        heap_caps_free(mybuf);
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

