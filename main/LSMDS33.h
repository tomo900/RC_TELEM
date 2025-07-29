#include <stdint.h>
#include "driver/i2c_master.h"

class LSMDS33{
    public:
        LSMDS33(uint8_t address, gpio_num_t i2c_GPIO_SCL, gpio_num_t i2c_GPIO_SDA);
        void Configure();
        void read_accGyro_values(int16_t *acc_x, int16_t *acc_y, int16_t *acc_z,
                                int16_t *gyro_x, int16_t *gyro_y, int16_t *gyro_z);
    private:
        uint8_t address; // I2C address of the LSMDS33 sensor
        i2c_master_bus_handle_t bus_handle; // Handle for the I2C bus
        i2c_master_dev_handle_t dev_handle; // Handle for the I2C device

};