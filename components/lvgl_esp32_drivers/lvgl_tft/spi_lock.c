/**
 * @file spi_lock.c
 */
#include "spi_lock.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"


/*********************
 *      DEFINES
 *********************/

/* Only enable a mutex if both devices use the same SPI bus */
#if CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1
#define USE_SPI_MUTEX
#endif
#else
#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_VSPI == 1
#define USE_SPI_MUTEX
#endif
#endif

/**********************
 *  STATIC VARIABLES
 **********************/
#ifdef USE_SPI_MUTEX
static SemaphoreHandle_t spi_mutex;
#endif

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void spi_mutex_init()
{
#ifdef USE_SPI_MUTEX
	spi_mutex = xSemaphoreCreateMutex();
#endif
}


void spi_lock()
{
#ifdef USE_SPI_MUTEX
	xSemaphoreTake(spi_mutex, portMAX_DELAY);
#endif
}


void spi_unlock()
{
#ifdef USE_SPI_MUTEX
	xSemaphoreGive(spi_mutex);
#endif
}