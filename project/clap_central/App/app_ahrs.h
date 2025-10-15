#ifndef _APP_AHRS_H
#define _APP_AHRS_H

#include "Mahony.h"
typedef struct{
    float x;
    float y;
    float z;
}axis_t;
typedef struct{
    float roll;
    float pitch;
    float yaw;
    axis_t acc;
    axis_t gyro;
    axis_t mag;
    mahony_t *algo_mahony;
}app_ahrs_t;

int8_t app_ahrs_init(app_ahrs_t *ahrs);

int8_t app_ahrs_update(app_ahrs_t *ahrs,float period_s);
#endif
