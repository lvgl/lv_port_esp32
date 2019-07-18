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

#define ENABLE_TOUCH_INPUT  TOUCH_SUPPORT

#define TP_SPI_MOSI 32
#define TP_SPI_MISO 35
#define TP_SPI_CLK  26
#define TP_SPI_CS   33


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
