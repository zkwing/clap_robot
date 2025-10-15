#include "app_ahrs.h"
#include "mahony.h"
#include "drv_init.h"

#define PI 3.14159265358979323846f
int8_t app_ahrs_init(app_ahrs_t *ahrs)
{
    mahony_init(ahrs->algo_mahony,1,0.0);
    return 0;
}

int8_t app_ahrs_update(app_ahrs_t *ahrs,float period_s)
{
    float roll,pitch,yaw;
    drv_adxl345_update(&g_acc_sensor);
//    drv_hmc5583l_update(&g_mag_sensor);
    drv_itg3205_update(&g_gyro_sensor);
    ahrs->acc.x = g_acc_sensor.scale_acc_data[0];
    ahrs->acc.y = g_acc_sensor.scale_acc_data[1];
    ahrs->acc.z = g_acc_sensor.scale_acc_data[2];
    ahrs->mag.x = g_mag_sensor.scale_mag_data[0];
    ahrs->mag.y = g_mag_sensor.scale_mag_data[1];
    ahrs->mag.z = g_mag_sensor.scale_mag_data[2];
    ahrs->gyro.x = g_gyro_sensor.scale_gyro_data[0];
    ahrs->gyro.y = g_gyro_sensor.scale_gyro_data[1];
    ahrs->gyro.z = g_gyro_sensor.scale_gyro_data[2];
    mahony_update(ahrs->algo_mahony,ahrs->gyro.x,ahrs->gyro.y,ahrs->gyro.z,
                                    ahrs->acc.x,ahrs->acc.y,ahrs->acc.z,
                                    ahrs->mag.x,ahrs->mag.y,ahrs->mag.z,period_s);
    mahony_get_euler(ahrs->algo_mahony,&roll,&pitch,&yaw); 
    ahrs->roll = roll * 180.0f / PI;
    ahrs->pitch = pitch * 180.0f / PI;
    ahrs->yaw = yaw * 180.0f / PI;
    return 0;
}
