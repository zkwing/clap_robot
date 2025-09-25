#include "drv_init.h"


drv_adxl345_t g_acc_sensor;
drv_itg3205_t g_gyro_sensor;
drv_hmc5583l_t g_mag_sensor;

void drv_init(void)
{
    drv_log_init(); 
    drv_adxl345_init(&g_acc_sensor,&g_acc_sensor_hw_i2c);
    drv_itg3205_init(&g_gyro_sensor,&g_gyro_sensor_hw_i2c);
    drv_hmc5583l_init(&g_mag_sensor,&g_mag_sensor_hw_i2c);

}
