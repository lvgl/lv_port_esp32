/* LVGL Example project
 * 
 * Basic project to test LVGL on ESP32 based projects.
 *
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

/* Littlevgl specific */
#include "lvgl/lvgl.h"
#include "lvgl_driver.h"
#ifdef CONFIG_LVGL_TFT_DISPLAY_MONOCHROME
#include "lv_theme_mono.h"
#endif
#include "lv_examples/lv_apps/demo/demo.h"

/*********************
 *      DEFINES
 *********************/
#define TAG "demo"

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void IRAM_ATTR lv_tick_task(void *arg);
void guiTask();


/**********************
 *   APPLICATION MAIN
 **********************/
void app_main() {
    
    //If you want to use a task to create the graphic, you NEED to create a Pinned task
    //Otherwise there can be problem such as memory corruption and so on
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 1);
}

static void IRAM_ATTR lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(portTICK_RATE_MS);
}

//Creates a semaphore to handle concurrent call to lvgl stuff
//If you wish to call *any* lvgl function from other threads/tasks
//you should lock on the very same semaphore!
SemaphoreHandle_t xGuiSemaphore;

void guiTask() {
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    lvgl_driver_init();

    static lv_color_t buf1[DISP_BUF_SIZE];
    static lv_color_t buf2[DISP_BUF_SIZE];
    static lv_disp_buf_t disp_buf;
    lv_disp_buf_init(&disp_buf, buf1, buf2, DISP_BUF_SIZE);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;
#ifdef CONFIG_LVGL_TFT_DISPLAY_MONOCHROME
    disp_drv.rounder_cb = disp_driver_rounder;
    disp_drv.set_px_cb = disp_driver_set_px;
#endif

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

#if defined CONFIG_LVGL_TFT_DISPLAY_MONOCHROME
    lv_theme_mono_init(0, NULL);
    lv_theme_set_current( lv_theme_get_mono() );
#endif

#if CONFIG_LVGL_TOUCH_CONTROLLER != TOUCH_CONTROLLER_NONE
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);
#endif

    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &lv_tick_task,
            /* name is optional, but may help identify the timer when debugging */
            .name = "periodic_gui"
    };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    //On ESP32 it's better to create a periodic task instead of esp_register_freertos_tick_hook
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 10*1000)); //10ms (expressed as microseconds)

#ifndef CONFIG_LVGL_TFT_DISPLAY_MONOCHROME
    demo_create();
#else
    /* use a pretty small demo for 128x64 monochrome displays */
    lv_obj_t * scr = lv_disp_get_scr_act(NULL);     /*Get the current screen*/

    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(scr, NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, "Hello\nworld!");

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0);

#endif // CONFIG_LVGL_TFT_DISPLAY_MONOCHROME
    while (1) {
        vTaskDelay(1);
        //Try to lock the semaphore, if success, call lvgl stuff
        if (xSemaphoreTake(xGuiSemaphore, (TickType_t)10) == pdTRUE) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
        }
    }

    //A task should NEVER return
    vTaskDelete(NULL);
}
