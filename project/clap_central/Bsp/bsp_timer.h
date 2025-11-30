#ifndef __BSP_TINER_H__
#define __BSP_TIMER_H__

/********************************************************************************************
* include:
********************************************************************************************/
#include "stm32h7xx_hal.h"
#include "stdint.h"

/********************************************************************************************
* Define:
********************************************************************************************/
#define MAX_CH_NUM  4
typedef enum {
  BSP_TIM_MODE_PWM = 0,
  BSP_TIM_MODE_IC_CNT,
  BSP_TIM_MODE_IC_PERIOD,
}tim_mode_t;
typedef struct{
    uint32_t channel;
    tim_mode_t mode;
    uint32_t pwm_v;
    uint32_t *ic_buf;
    uint16_t ic_buf_size;
}tim_ch_t;
typedef struct{
    TIM_HandleTypeDef *htim;
    DMA_HandleTypeDef* hdma;
    uint32_t period;
}timer_cfg_t;

typedef struct{
    timer_cfg_t hw_config; //定时器配置
    tim_ch_t tim_channel[MAX_CH_NUM];
    void (*cb)();
}timer_class_t;



/********************************************************************************************
* Datas:
********************************************************************************************/



/********************************************************************************************
* Function:
********************************************************************************************/
int8_t bsp_timer_init(timer_class_t *ptim);

int8_t bsp_timer_pwm_start(timer_class_t *ptim);
int8_t bsp_timer_pwm_stop(timer_class_t *ptim);
int8_t bsp_timer_pwm_set_duty(timer_class_t *ptim, uint32_t channel, uint32_t pwm_v);

int8_t bsp_timer_ic_start(timer_class_t *ptim);
int8_t bsp_timer_ic_stop(timer_class_t *ptim);
int8_t bsp_timer_ic_get_cnt(timer_class_t *ptim, uint32_t channel, uint32_t *cnt);
int8_t bsp_timer_ic_get_period(timer_class_t *ptim, uint32_t channel, uint32_t *period);

#endif
