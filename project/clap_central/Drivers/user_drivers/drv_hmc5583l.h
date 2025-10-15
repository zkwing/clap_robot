#ifndef __DRV_HMC5583L_H__
#define __DRV_HMC5583L_H__

#include "stdint.h"
#include "bsp_init.h"
#include "drv_log.h"
typedef struct{
    i2c_hw_class_t *i2c_interface;
    int16_t raw_mag_offset_data[3];
    int16_t raw_mag_data[3];   
    float scale_mag_data[3];
}drv_hmc5583l_t;

int8_t drv_hmc5583l_init(drv_hmc5583l_t *pdr, i2c_hw_class_t *pinf);
int8_t drv_hmc5583l_update(drv_hmc5583l_t *pdr);
int8_t drv_hmc5583l_cali(drv_hmc5583l_t *pdr);

#endif
