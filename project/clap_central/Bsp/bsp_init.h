#ifndef __BSP_INIT_H__
#define __BSP_INIT_H__
#include "stm32h7xx_hal.h"

//#define BSP_UART_ENABLE

#include "bsp_usart.h"
#include "bsp_i2c_hw.h"

extern uart_class_t g_debug_usart1;

int8_t bsp_init(void);



#endif
