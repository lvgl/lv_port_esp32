/**
@file jd79653a.c
@brief   GoodDisplay GDEW0154M09 e-paper display w/ FitiPower JD79653A
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
#include <driver/gpio.h>
#include <esp_log.h>

#include "disp_spi.h"
#include "jd79653a.h"

#define TAG "lv_jd79653a"

#define PIN_DC         CONFIG_LVGL_DISP_PIN_DC
#define PIN_DC_BIT     ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_DC)))
#define PIN_RST        CONFIG_LVGL_DISP_PIN_RST
#define PIN_RST_BIT    ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_RST)))
#define PIN_BUSY       CONFIG_LVGL_DISP_PIN_BUSY
#define PIN_BUSY_BIT   ((1ULL << (uint8_t)(CONFIG_LVGL_DISP_PIN_BUSY)))

typedef struct {
    uint8_t cmd;
    uint8_t data[3];
    size_t len;
} jd79653a_seq_t;

static const jd79653a_seq_t init_seq[] = {
        { 0x00, { 0xdf, 0x0e }, 2},                 // Panel settings
        { 0x4d, { 0x55 }, 1 },                             // Undocumented secret from demo code
        { 0xaa, { 0x0f }, 1 },                             // Undocumented secret from demo code
        { 0xe9, { 0x02 }, 1 },                             // Undocumented secret from demo code
        { 0xb6, { 0x11 }, 1 },                             // Undocumented secret from demo code
        { 0xf3, { 0x0a }, 1 },                             // Undocumented secret from demo code
        { 0x61, { 0xc8, 0x00, 0xc8 }, 3 },   // Resolution settings
        { 0x60, { 0x00 }, 1 },                             // TCON
        { 0x50, { 0x97 }, 1 },                             // VCOM sequence
        { 0xe3, { 0x00 }, 1 },                             // Power saving settings
        { 0x04, {}, 0 },                                          // Power ON!
};

static void jd79653a_spi_send_cmd(uint8_t cmd)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(PIN_DC, 0);	 // DC = 0 for command
    disp_spi_send_data(&cmd, 1);
}

static void jd79653a_spi_send_data(uint8_t *data, size_t len)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(PIN_DC, 1);  // DC = 1 for data
    disp_spi_send_data(data, len);
}

static void jd79653a_spi_send_fb(uint8_t *data, size_t len)
{
    disp_wait_for_pending_transactions();
    gpio_set_level(PIN_DC, 1);   // DC = 1 for data
    disp_spi_send_colors(data, len);
}

static void jd79653a_spi_send_seq(const jd79653a_seq_t *seq, size_t len)
{
    ESP_LOGI(TAG, "Writing init sequence, count %u", len);
    if (!seq || len < 1) return;
    for (size_t cmd_idx = 0; cmd_idx < len; cmd_idx++) {
        jd79653a_spi_send_cmd(seq[cmd_idx].cmd);
        if (seq[cmd_idx].len > 0) {
            jd79653a_spi_send_data((uint8_t *)seq[cmd_idx].data, seq[cmd_idx].len);
        }
    }
}

void jd79653a_init()
{
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
            .intr_type = GPIO_INTR_DISABLE,
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = PIN_BUSY_BIT,
            .pull_down_en = 0,
            .pull_up_en = 1,
    };
    ESP_ERROR_CHECK(gpio_config(&in_io_conf));

    // Hardware reset
    gpio_set_level(PIN_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(15)); // At least 10ms, leave 15ms for now just in case...
    gpio_set_level(PIN_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(120));

    // Dump in initialise sequence
    jd79653a_spi_send_seq(init_seq, sizeof(init_seq) / sizeof(jd79653a_seq_t));

    // Delay and check BUSY status here
    vTaskDelay(pdMS_TO_TICKS(100));
}
