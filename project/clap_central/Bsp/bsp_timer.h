#ifndef __BSP_TIMER_H__
#define __BSP_TIMER_H__

#include "stm32h7xx_hal.h"
#include "stdint.h"

/* 功能枚举 */
typedef enum {
    TIM_FUNC_PWM      = 0x01,
    TIM_FUNC_IC       = 0x02,
    TIM_FUNC_OC       = 0x04,
    TIM_FUNC_ENCODER  = 0x08,
    TIM_FUNC_BASIC    = 0x10,
} tim_func_mask_t;
/* 单个通道运行时参数 */
typedef struct {
    uint32_t  pulse;        /* PWM 占空比计数 / IC 捕获值 */
    float     duty;         /* 缓存 0~1 占空比 */
} tim_ch_t;

/* 定时器抽象层 */
typedef struct timer_class_t {
    TIM_HandleTypeDef *htim;
    uint32_t           func_mask;   /* 功能组合 */
    tim_ch_t           ch[4];       /* 常用四通道数据 */

    HAL_StatusTypeDef (*start)(struct timer_class_t *self, uint8_t ch_idx);
    HAL_StatusTypeDef (*stop) (struct timer_class_t *self, uint8_t ch_idx);
    void              (*set_duty)(struct timer_class_t *self, uint8_t ch_idx, float duty);
    uint32_t          (*get_count)(struct timer_class_t *self, uint8_t ch_idx);
    uint32_t          (*get_freq)(struct timer_class_t *self, uint8_t idx);
    float             (*get_duty)(struct timer_class_t *self, uint8_t idx);
    
} timer_class_t;
  
/* 工厂函数 */
void bsp_timer_init(timer_class_t *self,
                      TIM_HandleTypeDef *htim,
                      uint32_t func_mask);

/* 内联 helpers：把 ch_idx(0~3) 转成 HAL 通道号 */
static inline uint32_t idx2channel(uint8_t idx)
{
    const uint32_t tbl[] = {TIM_CHANNEL_1, TIM_CHANNEL_2,
                            TIM_CHANNEL_3, TIM_CHANNEL_4};
    return tbl[idx];
}


// int8_t bsp_timer_init(timer_class_t *timer, TIM_HandleTypeDef *htimer);
// int8_t bsp_timer_set_pwm(timer_class_t *timer, uint32_t channel, int16_t duty);
// uint32_t bsp_timer_get_count(timer_class_t *timer,uint32_t channel);
#endif
