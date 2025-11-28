#ifndef MOTOR_INTERFACE_H
#define MOTOR_INTERFACE_H

#include "motor_core.h"

// -------------------------- 电机通用接口（抽象类） --------------------------
typedef struct {
    // 基础操作
    StatusTypeDef (*Init)(void *handle);
    StatusTypeDef (*Start)(void *handle);
    StatusTypeDef (*Stop)(void *handle);
    StatusTypeDef (*SetTarget)(void *handle, int32_t target);
    StatusTypeDef (*GetState)(void *handle, Motor_StateTypeDef *state);
    
    // 周期控制（由调度任务调用）
    StatusTypeDef (*Control)(void *handle);
    
    // 扩展操作
    StatusTypeDef (*Recover)(void *handle);  // 故障恢复
} Motor_InterfaceTypeDef;

#endif  // MOTOR_INTERFACE_H