#ifndef __DRV_ITG3205_H__
#define __DRV_ITG3205_H__

#include "stdint.h"
#include "drv_init.h"

typedef struct{
    i2c_hw_class_t *i2c_interface;
    int16_t raw_gyro_data[3];
}drv_itg3205_t;

int8_t drv_itg3205_init(drv_itg3205_t *pdr,i2c_hw_class_t *pinf);

int8_t drv_itg3205_update(drv_itg3205_t *pdr);

#endif