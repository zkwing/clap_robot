#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__
#include "stm32h7xx_hal.h"

//#define BSP_UART_ENABLE

#include "bsp_usart.h"
#include "bsp_i2c_hw.h"


extern uart_class_t g_debug_usart1;
extern i2c_hw_class_t g_mag_sensor_hw_i2c;
extern i2c_hw_class_t g_acc_sensor_hw_i2c;
extern i2c_hw_class_t g_gyro_sensor_hw_i2c;

int8_t bsp_init(void);



#endif
