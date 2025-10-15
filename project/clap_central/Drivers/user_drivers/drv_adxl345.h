#ifndef __DRV_ADXL345_H__
#define __DRV_ADXL345_H__

#include "stdint.h"
#include "drv_init.h"

typedef struct{
    i2c_hw_class_t *i2c_interface;
    int16_t raw_acc_data[3];
}drv_adxl345_t;

int8_t drv_adxl345_init(drv_adxl345_t *pdr,i2c_hw_class_t *pinf);

int8_t drv_adxl345_update(drv_adxl345_t *pdr);

#endif
