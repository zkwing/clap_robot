#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__
#include "stm32h7xx_hal.h"

//#define BSP_UART_ENABLE

#include "bsp_usart.h"
#include "bsp_i2c_hw.h"
#include "bsp_timer.h"

extern uart_class_t g_debug_usart1;
extern timer_class_t g_wheel_pwm;
extern timer_class_t g_wheel_fg;

int8_t bsp_init(void);



#endif
