#ifndef __DRV_INIT_H__
#define __DRV_INIT_H__

#include "bsp_init.h"
#include "drv_log.h"
#include "drv_adxl345.h"
#include "drv_hmc5583l.h"
#include "drv_itg3205.h"

extern drv_adxl345_t g_acc_sensor;
extern drv_itg3205_t g_gyro_sensor;
extern drv_hmc5583l_t g_mag_sensor;


void drv_init(void);

#endif
