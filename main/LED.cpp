#include "LED.h"
#include "string.h"
#include "esp_log.h"
#include "led_strip.h"


#define LED_GPIO 48

LED::LED(char* name)
{
    strncpy(this->name, name, sizeof(this->name) - 1);
    this->name[sizeof(this->name) - 1] = '\0';

    /// LED strip common configuration
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_GPIO,  // The GPIO that connected to the LED strip's data line
        .max_leds = 1,                 // The number of LEDs in the strip,
    };

    /// SPI backend specific configuration
    led_strip_spi_config_t spi_config = {
        .clk_src = SPI_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .spi_bus = SPI2_HOST,           // SPI bus ID
        .flags = {
            .with_dma = true, // Using DMA can improve performance and help drive more LEDs
        }
    };
    

    ESP_ERROR_CHECK(led_strip_new_spi_device(&strip_config, &spi_config, &this->led_strip));
    led_strip_clear(led_strip);
    ESP_LOGI("LED", "LED initialized");
}

void LED::print_Name()
{
    ESP_LOGI("LED", "LED Name: %s", this->name);
}

void LED::set_Color(uint8_t red, uint8_t green, uint8_t blue)
{
    // Assuming you have a method to set the color of the LED strip
    // This is a placeholder for actual implementation
    uint32_t color = (red << 16) | (green << 8) | blue;
   
    led_strip_set_pixel (this->led_strip,0, red,green,blue);
    led_strip_refresh(this->led_strip);
}





