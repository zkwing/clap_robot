#include "bsp_timer.h"

/* ---------- pwm控制 ---------- */
static HAL_StatusTypeDef pwm_start(timer_class_t *self, uint8_t idx)
{
    uint32_t ch = idx2channel(idx);
    return HAL_TIM_PWM_Start(self->htim, ch);
}

static HAL_StatusTypeDef pwm_stop(timer_class_t *self, uint8_t idx)
{
    uint32_t ch = idx2channel(idx);
    return HAL_TIM_PWM_Stop(self->htim, ch);
}

static void pwm_set_duty(timer_class_t *self, uint8_t idx, float duty)
{
    uint32_t ch = idx2channel(idx);
    uint32_t arr = __HAL_TIM_GET_AUTORELOAD(self->htim) + 1;
    uint32_t pulse = (uint32_t)(arr * duty);
    self->ch[idx].duty  = duty;
    self->ch[idx].pulse = pulse;
    __HAL_TIM_SET_COMPARE(self->htim, ch, pulse);
}


static uint32_t pwm_get_count(timer_class_t *self, uint8_t idx)
{
    return self->ch[idx].pulse;
}

/* ---------- 捕获输入 ---------- */
static HAL_StatusTypeDef ic_start(timer_class_t *self, uint8_t idx)
{
    uint32_t ch = idx2channel(idx);
    return HAL_TIM_IC_Start(self->htim, ch);
}

static uint32_t ic_read_freq(timer_class_t *self, uint8_t idx)
{
    const uint32_t clk = HAL_RCC_GetPCLK1Freq(); /* 按实际总线改 */
    uint32_t psc = self->htim->Init.Prescaler + 1;
    uint32_t period = self->ch[idx].pulse; /* CCR1 周期 */
    return clk / (psc * period);
}

static float ic_read_duty(timer_class_t *self, uint8_t idx)
{
    uint32_t high = self->ch[idx + 1].pulse; /* CCR2 高电平 */
    uint32_t period = self->ch[idx].pulse;
    return (float)high / period;
}


/* ---------- 工厂函数 ---------- */
void timer_class_init(timer_class_t *self,
                      TIM_HandleTypeDef *htim,
                      uint32_t func_mask)
{
    self->htim      = htim;
    self->func_mask = func_mask;
    memset(self->ch, 0, sizeof(self->ch));

    HAL_TIM_Base_Start(htim);

    if (func_mask & TIM_FUNC_PWM) {
        self->start   = pwm_start;
        self->stop    = pwm_stop;
        self->set_duty = pwm_set_duty;
        self->get_count  = pwm_get_count;
    }
    if (func_mask & TIM_FUNC_IC) {
        self->start   = ic_start;
        self->get_freq = ic_read_freq;
        self->get_duty = ic_read_duty;
    }
    
}