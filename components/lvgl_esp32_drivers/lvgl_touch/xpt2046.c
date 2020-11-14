/**
 * @file XPT2046.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "xpt2046.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "tp_spi.h"
#include <stddef.h>

/*********************
 *      DEFINES
 *********************/
#define TAG "XPT2046"

#define CMD_X_READ  0b10010000
#define CMD_Y_READ  0b11010000

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    XPT2046_STATE_IDLE,
    XPT2046_STATE_PRESSED
} xpt2046_state_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void xpt2046_corr(int16_t * x, int16_t * y);
static void xpt2046_avg(int16_t * x, int16_t * y);

static xpt2046_state_t xpt2046_is_pressed(void);

/**********************
 *  STATIC VARIABLES
 **********************/
int16_t avg_buf_x[XPT2046_AVG];
int16_t avg_buf_y[XPT2046_AVG];
uint8_t avg_last;

#if defined (CONFIG_LV_TOUCH_DETECTION_Z_COORD)
uint32_t z_coord_pressure_threshold = CONFIG_LV_TOUCH_Z_COORD_THRESHOLD;
#endif

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * Initialize the XPT2046
 */
void xpt2046_init(void)
{
#if defined (CONFIG_LV_TOUCH_DETECTION_IRQ_SIGNAL)
    gpio_config_t irq_config = {
        .pin_bit_mask = BIT64(XPT2046_IRQ),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    ESP_LOGI(TAG, "XPT2046 Initialization");

    esp_err_t ret = gpio_config(&irq_config);
    assert(ret == ESP_OK);
#endif
}

/**
 * Get the current position and state of the touchpad
 * @param data store the read data here
 * @return false: because no more data to be read
 */
bool xpt2046_read(lv_indev_drv_t * drv, lv_indev_data_t * data)
{
    static int16_t last_x = 0;
    static int16_t last_y = 0;
    bool valid = true;

    int16_t x = 0;
    int16_t y = 0;

    volatile xpt2046_state_t touch_state = xpt2046_is_pressed();

    if (XPT2046_STATE_PRESSED == touch_state) {
		uint8_t data[2] = {0};
		
		tp_spi_read_reg(CMD_X_READ, data, 2);
		x = (data[0] << 8) | data[1];
		
		tp_spi_read_reg(CMD_Y_READ, data, 2);
		y = (data[0] << 8) | data[1];
		ESP_LOGI(TAG, "P(%d,%d)", x, y);
		
        /*Normalize Data back to 12-bits*/
        x = x >> 4;
        y = y >> 4;
        ESP_LOGI(TAG, "P_norm(%d,%d)", x, y);
		
        xpt2046_corr(&x, &y);
        xpt2046_avg(&x, &y);
        last_x = x;
        last_y = y;

		ESP_LOGI(TAG, "x = %d, y = %d", x, y);
    } else {
        x = last_x;
        y = last_y;
        avg_last = 0;
        valid = false;
    }

    data->point.x = x;
    data->point.y = y;
    data->state = valid == false ? LV_INDEV_STATE_REL : LV_INDEV_STATE_PR;

    return false;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void xpt2046_corr(int16_t * x, int16_t * y)
{
#if XPT2046_XY_SWAP != 0
	int16_t swap_tmp;
    swap_tmp = *x;
    *x = *y;
    *y = swap_tmp;
#endif

    if((*x) > XPT2046_X_MIN)(*x) -= XPT2046_X_MIN;
    else(*x) = 0;

    if((*y) > XPT2046_Y_MIN)(*y) -= XPT2046_Y_MIN;
    else(*y) = 0;

    (*x) = (uint32_t)((uint32_t)(*x) * LV_HOR_RES) /
           (XPT2046_X_MAX - XPT2046_X_MIN);

    (*y) = (uint32_t)((uint32_t)(*y) * LV_VER_RES) /
           (XPT2046_Y_MAX - XPT2046_Y_MIN);

#if XPT2046_X_INV != 0
    (*x) =  LV_HOR_RES - (*x);
#endif

#if XPT2046_Y_INV != 0
    (*y) =  LV_VER_RES - (*y);
#endif


}


static void xpt2046_avg(int16_t * x, int16_t * y)
{
    /*Shift out the oldest data*/
    uint8_t i;
    for(i = XPT2046_AVG - 1; i > 0 ; i--) {
        avg_buf_x[i] = avg_buf_x[i - 1];
        avg_buf_y[i] = avg_buf_y[i - 1];
    }

    /*Insert the new point*/
    avg_buf_x[0] = *x;
    avg_buf_y[0] = *y;
    if(avg_last < XPT2046_AVG) avg_last++;

    /*Sum the x and y coordinates*/
    int32_t x_sum = 0;
    int32_t y_sum = 0;
    for(i = 0; i < avg_last ; i++) {
        x_sum += avg_buf_x[i];
        y_sum += avg_buf_y[i];
    }

    /*Normalize the sums*/
    (*x) = (int32_t)x_sum / avg_last;
    (*y) = (int32_t)y_sum / avg_last;
}

/* Determine if the screen is being touched
 * It can determine it in two ways:
 * - Reading the IRQ signal.
 * - Reading the Z coordinate of the touch sensor.
 *
 * NOTE: Currently only reading the IRQ is supported.
 * TODO: Let the user choose one of the options via menuconfig. */
static xpt2046_state_t xpt2046_is_pressed(void)
{
    xpt2046_state_t retval = XPT2046_STATE_IDLE;

#if defined (CONFIG_LV_TOUCH_DETECTION_IRQ_SIGNAL)
    uint8_t irq = gpio_get_level(XPT2046_IRQ);

    if (irq == 0) {
        retval = XPT2046_STATE_PRESSED;
    }
#else
    /* Get z coord */
    uint32_t z_coord_read = 0;

    if (z_coord_pressure_threshold <= z_coord_read) {
        retval = XPT2046_STATE_PRESSED;
    }
#endif

    return retval;
}
