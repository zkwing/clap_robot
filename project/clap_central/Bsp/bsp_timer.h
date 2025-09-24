#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stm32h7xx_hal.h"
#include "stdint.h"

typedef struct 
{
    TIM_HandleTypeDef *htimer;

} timer_class_t;

int8_t bsp_timer_init(timer_class_t *timer, TIM_HandleTypeDef *htimer);
int8_t bsp_timer_set_pwm(timer_class_t *timer, uint32_t channel, int16_t duty);
uint32_t bsp_timer_get_count(timer_class_t *timer,uint32_t channel);
#endif
