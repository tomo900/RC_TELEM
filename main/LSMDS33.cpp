
#include "LSMDS33.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define I2C_TIMEOUT 100

LSMDS33::LSMDS33(uint8_t address, gpio_num_t i2c_GPIO_SCL, gpio_num_t i2c_GPIO_SDA)
{
    this->address = address;
    // Initialize the I2C bus and device
    i2c_master_bus_config_t bus_config = {
        .i2c_port = -1,
        .sda_io_num = i2c_GPIO_SDA,
        .scl_io_num = i2c_GPIO_SCL,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };
    
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &this->bus_handle));

    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 100000,
    };
    
    ESP_ERROR_CHECK(i2c_master_bus_add_device(this->bus_handle, &device_config, &this->dev_handle));
}

void LSMDS33::Configure()
{
    // Configure accelerometer to 1.66 kHz (ODR_XL = 0b101)
    // CTRL1_XL (0x10): ODR_XL[3:0]=0b1010 (1.66kHz), FS_XL=0b00 (±2g), BW_XL=0b00 (400Hz)
    uint8_t acc_cfg[2] = {0x10, 0b10100000}; // ODR_XL=1.66kHz, ±2g, 400Hz BW
    ESP_ERROR_CHECK(i2c_master_transmit(this->dev_handle, acc_cfg, 2, I2C_TIMEOUT));

    // Configure gyroscope to 1.66 kHz (ODR_G = 0b101)
    // CTRL2_G (0x11): ODR_G[3:0]=0b1010 (1.66kHz), FS_G=0b00 (±245 dps)
    uint8_t gyro_cfg[2] = {0x11, 0b10100000}; // ODR_G=1.66kHz, ±245dps
    ESP_ERROR_CHECK(i2c_master_transmit(this->dev_handle, gyro_cfg, 2, I2C_TIMEOUT));
}

void LSMDS33::read_accGyro_values(int16_t *acc_x,int16_t *acc_y,int16_t *acc_z,
                                  int16_t *gyro_x,int16_t *gyro_y,int16_t *gyro_z)
{
    // Read accelerometer values from the LSMDS33 sensor
    uint8_t acc_data[6];
    uint8_t acc_reg = 0x28; // Starting register address for accelerometer (OUTX_L_XL)
    ESP_ERROR_CHECK(i2c_master_transmit_receive(this->dev_handle, &acc_reg, 1, acc_data, 6, I2C_TIMEOUT));

    // Read gyroscope values from the LSMDS33 sensor
    uint8_t gyro_data[6];
    uint8_t gyro_reg = 0x22; // Starting register address for gyroscope (OUTX_L_G)
    ESP_ERROR_CHECK(i2c_master_transmit_receive(this->dev_handle, &gyro_reg, 1, gyro_data, 6, I2C_TIMEOUT));

    // Process the data to extract accelerometer and gyroscope values
    *acc_x = (acc_data[1] << 8) | acc_data[0];
    *acc_y = (acc_data[3] << 8) | acc_data[2];
    *acc_z = (acc_data[5] << 8) | acc_data[4];
    *gyro_x = (gyro_data[1] << 8) | gyro_data[0];
    *gyro_y = (gyro_data[3] << 8) | gyro_data[2];
    *gyro_z = (gyro_data[5] << 8) | gyro_data[4];
}

