#ifndef __DRV_LOG_H__
#define __DRV_LOG_H__
#include "elog.h"
#include "bsp_init.h"

void drv_log_init(void);
void jcom_float_print(uint8_t ch,float data1,float data2,float data3);
#endif
