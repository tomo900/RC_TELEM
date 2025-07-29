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
#include "LSMDS33.h"


extern "C" void app_main(void)
{
    char test[] = "test";
    
    LED led(test);
    LSMDS33 imu(0x6A, (gpio_num_t)48, (gpio_num_t)47); // Example I2C address and GPIOs
    imu.Configure(); // Configure the IMU sensor
    while (1) {
        int16_t ax, ay, az, gx, gy, gz;
        imu.read_accGyro_values(&ax, &ay, &az, &gx, &gy, &gz);
        ESP_LOGI("IMU", "Accel: ax=%d ay=%d az=%d | Gyro: gx=%d gy=%d gz=%d", ax, ay, az, gx, gy, gz);
        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
}
