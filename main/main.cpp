/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"
#include "LED.h"



extern "C" void app_main(void)
{
    char test[] = "test";
    
    LED led(test);
    

    while (1) {
        
        led.print_Name();
        uint8_t div =10;
        led.set_Color(255/div, 0, 100/div); // Set color to red
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
        led.set_Color(0, 0, 0); // Set color to red
        vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
    }
}
