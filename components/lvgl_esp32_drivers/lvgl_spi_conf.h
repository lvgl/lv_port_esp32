/**
 * @file lvgl_spi_conf.h
 *
 */

#ifndef LVGL_SPI_CONF_H
#define LVGL_SPI_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/
// DISPLAY PINS
#define DISP_SPI_MOSI CONFIG_LVGL_DISP_SPI_MOSI
#define DISP_SPI_CLK CONFIG_LVGL_DISP_SPI_CLK
#define DISP_SPI_CS CONFIG_LVGL_DISP_SPI_CS

// TOUCHPAD PINS
#define TP_SPI_MOSI CONFIG_LVGL_TOUCH_SPI_MOSI
#define TP_SPI_MISO CONFIG_LVGL_TOUCH_SPI_MISO
#define TP_SPI_CLK  CONFIG_LVGL_TOUCH_SPI_CLK
#define TP_SPI_CS   CONFIG_LVGL_TOUCH_SPI_CS

#define ENABLE_TOUCH_INPUT  CONFIG_LVGL_ENABLE_TOUCH

#if CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1
#define TFT_SPI_HOST HSPI_HOST
#else
#define TFT_SPI_HOST VSPI_HOST
#endif /*CONFIG_LVGL_TFT_DISPLAY_SPI_HSPI == 1*/

#if CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1
#define TOUCH_SPI_HOST HSPI_HOST
#else
#define TOUCH_SPI_HOST VSPI_HOST
#endif /*CONFIG_LVGL_TOUCH_CONTROLLER_SPI_HSPI == 1*/

// Detect the use of a shared SPI Bus and verify the user specified the same SPI bus for both touch and tft
#if (CONFIG_LVGL_TOUCH_CONTROLLER == 1 || CONFIG_LVGL_TOUCH_CONTROLLER == 3) && TP_SPI_MOSI == DISP_SPI_MOSI && TP_SPI_CLK == DISP_SPI_CLK
#if TFT_SPI_HOST != TOUCH_SPI_HOST
#error You must specifiy the same SPI host for both display and input driver
#endif

#define SHARED_SPI_BUS
#endif

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/


/**********************
 *      MACROS
 **********************/


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LVGL_SPI_CONF_H*/
