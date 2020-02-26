/**
 * @file disp_spi.h
 *
 */

#ifndef DISP_SPI_H
#define DISP_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include <stdbool.h>
#include <driver/spi_master.h>

/*********************
 *      DEFINES
 *********************/

#define DISP_SPI_MOSI CONFIG_LVGL_DISP_SPI_MOSI
#define DISP_SPI_CLK CONFIG_LVGL_DISP_SPI_CLK
#define DISP_SPI_CS CONFIG_LVGL_DISP_SPI_CS


/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void disp_spi_init(void);
void disp_spi_add_device(spi_host_device_t host);
void disp_spi_add_device_config(spi_host_device_t host, spi_device_interface_config_t *devcfg);
void disp_spi_send_data(uint8_t * data, uint16_t length);
void disp_spi_send_colors(uint8_t * data, uint16_t length);
bool disp_spi_is_busy(void);

/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*DISP_SPI_H*/
