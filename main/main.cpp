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
#include "driver/i2c_master.h"
static i2c_master_bus_handle_t bus_handle;

void init_I2c_Bus(i2c_master_bus_handle_t* bus_handle_ptr){
        // Initialize the I2C bus and device
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0, // Use I2C port 0
        .sda_io_num = (gpio_num_t)47,
        .scl_io_num = (gpio_num_t)48,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle_ptr));

}


extern "C" void app_main(void)
{
   
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0, // Use I2C port 0
        .sda_io_num = (gpio_num_t)47,
        .scl_io_num = (gpio_num_t)48,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));


    LSMDS33 imu(&bus_handle,0x6A); // Example I2C address and GPIOs
    imu.Configure(); // Configure the IMU sensor
    while (1) {
        float ax, ay, az, gx, gy, gz;
        imu.read_accGyro_values(&ax, &ay, &az, &gx, &gy, &gz);
        ESP_LOGI("IMU", "Accel: ax=%f ay=%f az=%f | Gyro: gx=%f gy=%f gz=%f", ax, ay, az, gx, gy, gz);
        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
}
