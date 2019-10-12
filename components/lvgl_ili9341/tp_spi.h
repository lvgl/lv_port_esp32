/**
 * @file tp_spi.h
 *
 */

#ifndef TP_SPI_H
#define TP_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>

/*********************
 *      DEFINES
 *********************/

#define ENABLE_TOUCH_INPUT  CONFIG_LVGL_ENABLE_TOUCH

#define TP_SPI_MOSI CONFIG_LVGL_TOUCH_SPI_MOSI
#define TP_SPI_MISO CONFIG_LVGL_TOUCH_SPI_MISO
#define TP_SPI_CLK  CONFIG_LVGL_TOUCH_SPI_CLK
#define TP_SPI_CS   CONFIG_LVGL_TOUCH_SPI_CS


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void tp_spi_init(void);
uint8_t tp_spi_xchg(uint8_t data_send);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*TP_SPI_H*/
