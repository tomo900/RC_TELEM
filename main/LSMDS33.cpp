
#include "LSMDS33.h"
#include "esp_log.h"
#include "driver/i2c_master.h"

#define I2C_TIMEOUT 100

LSMDS33::LSMDS33(i2c_master_bus_handle_t* bus_handle_ptr, uint8_t address)
{
    this->address = address;

    i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 100000,
    };
    
    ESP_ERROR_CHECK(i2c_master_bus_add_device(*bus_handle_ptr, &device_config, &this->dev_handle));
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

void LSMDS33::read_accGyro_values(float *acc_x, float *acc_y, float *acc_z,
                                  float *gyro_x, float *gyro_y, float *gyro_z)
{
    // Read accelerometer values from the LSMDS33 sensor
    uint8_t acc_data[6];
    uint8_t acc_reg = 0x28; // Starting register address for accelerometer (OUTX_L_XL)
    ESP_ERROR_CHECK(i2c_master_transmit_receive(this->dev_handle, &acc_reg, 1, acc_data, 6, I2C_TIMEOUT));

    // Read gyroscope values from the LSMDS33 sensor
    uint8_t gyro_data[6];
    uint8_t gyro_reg = 0x22; // Starting register address for gyroscope (OUTX_L_G)
    ESP_ERROR_CHECK(i2c_master_transmit_receive(this->dev_handle, &gyro_reg, 1, gyro_data, 6, I2C_TIMEOUT));

    // Convert raw data to signed 16-bit values
    int16_t raw_acc_x = (acc_data[1] << 8) | acc_data[0];
    int16_t raw_acc_y = (acc_data[3] << 8) | acc_data[2];
    int16_t raw_acc_z = (acc_data[5] << 8) | acc_data[4];
    int16_t raw_gyro_x = (gyro_data[1] << 8) | gyro_data[0];
    int16_t raw_gyro_y = (gyro_data[3] << 8) | gyro_data[2];
    int16_t raw_gyro_z = (gyro_data[5] << 8) | gyro_data[4];

    // Sensitivity values from datasheet
    const float ACC_SENSITIVITY = 0.000061f; // g/LSB for ±2g
    const float GYRO_SENSITIVITY = 0.00875f; // dps/LSB for ±245 dps

    // Convert to physical units
    *acc_x = raw_acc_x * ACC_SENSITIVITY;
    *acc_y = raw_acc_y * ACC_SENSITIVITY;
    *acc_z = raw_acc_z * ACC_SENSITIVITY;
    *gyro_x = raw_gyro_x * GYRO_SENSITIVITY;
    *gyro_y = raw_gyro_y * GYRO_SENSITIVITY;
    *gyro_z = raw_gyro_z * GYRO_SENSITIVITY;
}

