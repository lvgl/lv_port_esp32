/**
 * @file st7789.c
 *
 * Mostly taken from lbthomsen/esp-idf-littlevgl github.
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "st7789.h"

#include "disp_spi.h"
#include "driver/gpio.h"

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
static void st7789_send_cmd(uint8_t cmd);
static void st7789_send_data(void *data, uint16_t length);
static void st7789_send_color(void *data, uint16_t length);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void st7789_init(void)
{
    lcd_init_cmd_t st7789_init_cmds[] = {
        {0xCF, {0x00, 0x83, 0X30}, 3},
        {0xED, {0x64, 0x03, 0X12, 0X81}, 4},
        {ST7789_PWCTRL2, {0x85, 0x01, 0x79}, 3},
        {0xCB, {0x39, 0x2C, 0x00, 0x34, 0x02}, 5},
        {0xF7, {0x20}, 1},
        {0xEA, {0x00, 0x00}, 2},
        {ST7789_LCMCTRL, {0x26}, 1}
        {ST7789_IDSET, {0x11}, 1}
        {ST7789_VCMOFSET, {0x35, 0x3E}, 2}
        {ST7789_CABCCTRL, {0xBE}, 1}
        {ST7789_MADCTL, {0x28}, 1}
        {ST7789_COLMOD, {0x55}, 1},
        {ST7789_RGBCTRL, {0x00, 0x1B}, 2},
        {0xF2, {0x08}, 1},
        {ST7789_GAMSET, {0x01}, 1},
        {ST7789_PVGAMCTRL, {0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0X87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00}, 15},
        {ST7789_NVGAMCTRL, {0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F}, 15},
        {ST7789_CASET, {0x00, 0x00, 0x00, 0xEF}, 4},
        {ST7789_RASET, {0x00, 0x00, 0x01, 0x3f}, 4},
        {ST7789_RAMWR, {0}, 0},
        {ST7789_GCTRL, {0x07}, 1},
        {0xB6, {0x0A, 0x82, 0x27, 0x00}, 4},
        {ST7789_SLPOUT, {0}, 0x80},
        {ST7789_DISPON, {0}, 0x80},
        {0, {0}, 0xff},
    };

    //Initialize non-SPI GPIOs
    gpio_set_direction(ST7789_DC, GPIO_MODE_OUTPUT);
    gpio_set_direction(ST7789_RST, GPIO_MODE_OUTPUT);
    
#if ST7789_ENABLE_BACKLIGHT_CONTROL
    gpio_set_direction(ST7789_BCKL, GPIO_MODE_OUTPUT);
#endif

    //Reset the display
    gpio_set_level(ST7789_RST, 0);
    vTaskDelay(100 / portTICK_RATE_MS);
    gpio_set_level(ST7789_RST, 1);
    vTaskDelay(100 / portTICK_RATE_MS);

    printf("ST7789 initialization.\n");

    //Send all the commands
    uint16_t cmd = 0;
    while (st7789_init_cmds[cmd].databytes!=0xff) {
        st7789_send_cmd(st7789_init_cmds[cmd].cmd);
        st7789_send_data(st7789_init_cmds[cmd].data, st7789_init_cmds[cmd].databytes&0x1F);
        if (st7789_init_cmds[cmd].databytes & 0x80) {
                vTaskDelay(100 / portTICK_RATE_MS);
        }
        cmd++;
    }

    st7789_enable_backlight(true);
}

void st7789_enable_backlight(bool backlight)
{
#if ST7789_ENABLE_BACKLIGHT_CONTROL
    printf("%s backlight.\n", backlight ? "Enabling" : "Disabling");
    uint32_t tmp = 0;

#if (ST7789_BCKL_ACTIVE_LVL==1)
    tmp = backlight ? 1 : 0;
#else
    tmp = backlight ? 0 : 1;
#endif

    gpio_set_level(ST7789_BCKL, tmp);
#endif
}


void st7789_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map)
{
    uint8_t data[4] = {0};

    /*Column addresses*/
    st7789_send_cmd(ST7789_CASET);
    data[0] = (area->x1 >> 8) & 0xFF;
    data[1] = area->x1 & 0xFF;
    data[2] = (area->x2 >> 8) & 0xFF;
    data[3] = area->x2 & 0xFF;
    st7789_send_data(data, 4);

    /*Page addresses*/
    st7789_send_cmd(ST7789_RASET);
    data[0] = (area->y1 >> 8) & 0xFF;
    data[1] = area->y1 & 0xFF;
    data[2] = (area->y2 >> 8) & 0xFF;
    data[3] = area->y2 & 0xFF;
    st7789_send_data(data, 4);

    /*Memory write*/
    st7789_send_cmd(ST7789_RAMWR);

    uint32_t size = lv_area_get_width(area) * lv_area_get_height(area);

    st7789_send_color((void*)color_map, size * 2);

}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void st7789_send_cmd(uint8_t cmd)
{
    gpio_set_level(ST7789_DC, 0);
    disp_spi_send_data(&cmd, 1);
}

static void st7789_send_data(void * data, uint16_t length)
{
    gpio_set_level(ST7789_DC, 1);
    disp_spi_send_data(data, length);
}

static void st7789_send_color(void * data, uint16_t length)
{
    gpio_set_level(ST7789_DC, 1);
    disp_spi_send_colors(data, length);
}
