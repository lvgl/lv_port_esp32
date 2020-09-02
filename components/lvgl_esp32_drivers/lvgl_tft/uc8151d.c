/**
    @file uc8151d.c
    @brief   GoodDisplay GDEW0154M10 DES e-paper display w/ UltraChip UC8151D
    @version 1.0
    @date    2020-08-28
    @author  Jackson Ming Hu <huming2207@gmail.com>


    @section LICENSE

    MIT License

    Copyright (c) 2020 Jackson Ming Hu

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute,
    sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <driver/gpio.h>
#include <esp_log.h>

#include "disp_driver.h"
#include "uc8151d.h"

#define TAG "lv_uc8151d"

#define PIN_DC              CONFIG_LVGL_DISP_PIN_DC
#define PIN_DC_BIT          ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_DC)))
#define PIN_RST             CONFIG_LVGL_DISP_PIN_RST
#define PIN_RST_BIT         ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_RST)))
#define PIN_BUSY            CONFIG_LVGL_DISP_PIN_BUSY
#define PIN_BUSY_BIT        ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_BUSY)))
#define EVT_BUSY            (1UL << 0UL)
#define EPD_WIDTH           CONFIG_LVGL_DISPLAY_WIDTH
#define EPD_HEIGHT          CONFIG_LVGL_DISPLAY_HEIGHT
#define EPD_ROW_LEN         (EPD_HEIGHT / 8u)

#define BIT_SET(a, b)       ((a) |= (1U << (b)))
#define BIT_CLEAR(a, b)     ((a) &= ~(1U << (b)))

typedef struct
{
    uint8_t cmd;
    uint8_t data[3];
    size_t len;
} uc8151d_seq_t;

#define EPD_SEQ_LEN(x) ((sizeof(x) / sizeof(uc8151d_seq_t)))

static EventGroupHandle_t uc8151d_evts = NULL;

static void IRAM_ATTR uc8151d_busy_intr(void *arg)
{
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xResult = xEventGroupSetBitsFromISR(uc8151d_evts, EVT_BUSY, &xHigherPriorityTaskWoken);
    if (xResult == pdPASS) {
        portYIELD_FROM_ISR();
    }
}

static esp_err_t uc8151d_wait_busy(uint32_t timeout_ms)
{
    uint32_t wait_ticks = (timeout_ms == 0 ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms));
    EventBits_t bits = xEventGroupWaitBits(uc8151d_evts,
                                           EVT_BUSY, // Wait for busy bit
                                           pdTRUE, pdTRUE,       // Clear on exit, wait for all
                                           wait_ticks);         // Timeout

    return ((bits & EVT_BUSY) != 0) ? ESP_OK : ESP_ERR_TIMEOUT;
}

void uc8151d_init()
{
    // Initialise event group
    uc8151d_evts = xEventGroupCreate();
    if (!uc8151d_evts) {
        ESP_LOGE(TAG, "Failed when initialising event group!");
        return;
    }

    // Setup output pins, output (PP)
    gpio_config_t out_io_conf = {
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_OUTPUT,
            .pin_bit_mask = PIN_DC_BIT | PIN_RST_BIT,
            .pull_down_en = 0,
            .pull_up_en = 0,
    };
    ESP_ERROR_CHECK(gpio_config(&out_io_conf));

    // Setup input pin, pull-up, input
    gpio_config_t in_io_conf = {
            .intr_type = GPIO_INTR_POSEDGE,
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = PIN_BUSY_BIT,
            .pull_down_en = 0,
            .pull_up_en = 1,
    };
    ESP_ERROR_CHECK(gpio_config(&in_io_conf));
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_BUSY, uc8151d_busy_intr, (void *) PIN_BUSY);

    // Hardware reset
    gpio_set_level(PIN_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(20)); // At least 10ms, leave 15ms for now just in case...
    gpio_set_level(PIN_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(20));
}
