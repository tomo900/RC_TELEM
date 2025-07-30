#include <stdint.h>
#include "driver/i2c_master.h"

class LSMDS33{
    public:
        LSMDS33(i2c_master_bus_handle_t* bus_handle_ptr, uint8_t address);
        void Configure();
        void read_accGyro_values(float *acc_x, float *acc_y, float *acc_z,
                                float *gyro_x, float *gyro_y, float *gyro_z);
    private:
        uint8_t address; // I2C address of the LSMDS33 sensor
        i2c_master_dev_handle_t dev_handle; // Handle for the I2C device

};