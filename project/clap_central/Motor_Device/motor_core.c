#include "motor_core.h"
#include "string.h"

// -------------------------- 全局变量 --------------------------
Motor_ManagerTypeDef g_motor_manager = {0};  // 电机管理器单例
static Motor_FaultCallbackTypeDef g_fault_callback = NULL;  // 故障回调函数

// -------------------------- 私有函数声明 --------------------------
static StatusTypeDef Motor_StateSwitch(Motor_InterfaceTypeDef *motor, Motor_StateEnum target_state);
static uint8_t Motor_GetIndex(Motor_InterfaceTypeDef *motor);

// -------------------------- 电机管理器实现 --------------------------
StatusTypeDef Motor_ManagerInit(void) {
    memset(&g_motor_manager, 0, sizeof(Motor_ManagerTypeDef));
    return STATUS_OK;
}

StatusTypeDef Motor_Register(Motor_InterfaceTypeDef *motor) {
    if (motor == NULL || g_motor_manager.motor_count >= MAX_MOTOR_NUM) {
        return STATUS_INVALID_PARAM;
    }

    // 检查是否已注册
    for (uint8_t i = 0; i < g_motor_manager.motor_count; i++) {
        if (g_motor_manager.motor_list[i] == motor) {
            return STATUS_BUSY;
        }
    }

    // 初始化电机并加入列表
    if (motor->Init(motor) != STATUS_OK) {
        return STATUS_ERROR;
    }
    g_motor_manager.motor_list[g_motor_manager.motor_count++] = motor;
    return STATUS_OK;
}

StatusTypeDef Motor_Unregister(Motor_InterfaceTypeDef *motor) {
    if (motor == NULL) {
        return STATUS_INVALID_PARAM;
    }

    uint8_t index = Motor_GetIndex(motor);
    if (index == MAX_MOTOR_NUM) {
        return STATUS_ERROR;
    }

    // 停止电机并移除
    Motor_Stop(motor);
    for (uint8_t i = index; i < g_motor_manager.motor_count - 1; i++) {
        g_motor_manager.motor_list[i] = g_motor_manager.motor_list[i + 1];
    }
    g_motor_manager.motor_count--;
    return STATUS_OK;
}

// -------------------------- 电机状态机实现 --------------------------
static StatusTypeDef Motor_StateSwitch(Motor_InterfaceTypeDef *motor, Motor_StateEnum target_state) {
    Motor_StateTypeDef state;
    if (motor->GetState(motor, &state) != STATUS_OK) {
        return STATUS_ERROR;
    }

    // 状态切换合法性检查
    switch (target_state) {
        case MOTOR_STATE_RUNNING:
            if (state.state != MOTOR_STATE_IDLE) {
                return STATUS_BUSY;
            }
            break;
        case MOTOR_STATE_STOPPED:
            if (state.state == MOTOR_STATE_UNINIT || state.state == MOTOR_STATE_FAULT) {
                return STATUS_ERROR;
            }
            break;
        case MOTOR_STATE_FAULT:
            // 故障状态可强制切换
            break;
        default:
            return STATUS_INVALID_PARAM;
    }

    // 执行状态切换操作
    switch (target_state) {
        case MOTOR_STATE_RUNNING:
            return motor->Start(motor);
        case MOTOR_STATE_STOPPED:
            return motor->Stop(motor);
        case MOTOR_STATE_FAULT:
            motor->Stop(motor);
            return STATUS_OK;
        default:
            return STATUS_OK;
    }
}

// -------------------------- 电机控制API实现 --------------------------
StatusTypeDef Motor_Start(Motor_InterfaceTypeDef *motor) {
    if (motor == NULL) {
        return STATUS_INVALID_PARAM;
    }
    return Motor_StateSwitch(motor, MOTOR_STATE_RUNNING);
}

StatusTypeDef Motor_Stop(Motor_InterfaceTypeDef *motor) {
    if (motor == NULL) {
        return STATUS_INVALID_PARAM;
    }
    return Motor_StateSwitch(motor, MOTOR_STATE_STOPPED);
}

StatusTypeDef Motor_SetTarget(Motor_InterfaceTypeDef *motor, int32_t target) {
    if (motor == NULL) {
        return STATUS_INVALID_PARAM;
    }
    return motor->SetTarget(motor, target);
}

StatusTypeDef Motor_GetState(Motor_InterfaceTypeDef *motor, Motor_StateTypeDef *state) {
    if (motor == NULL || state == NULL) {
        return STATUS_INVALID_PARAM;
    }
    return motor->GetState(motor, state);
}

// -------------------------- 故障管理实现 --------------------------
void Motor_RegisterFaultCallback(Motor_FaultCallbackTypeDef callback) {
    if (callback != NULL) {
        g_fault_callback = callback;
    }
}

void Motor_FaultHandle(Motor_InterfaceTypeDef *motor, Motor_FaultEnum fault) {
    if (motor == NULL || fault == MOTOR_FAULT_NONE) {
        return;
    }

    // 切换到故障状态
    Motor_StateSwitch(motor, MOTOR_STATE_FAULT);

    // 触发故障回调
    if (g_fault_callback != NULL) {
        g_fault_callback(motor, fault);
    }
}

// -------------------------- 控制调度实现 --------------------------
void Motor_ControlSchedulerTask(void) {
    g_motor_manager.tick_count += MOTOR_CONTROL_PERIOD;

    // 遍历所有电机执行控制逻辑
    for (uint8_t i = 0; i < g_motor_manager.motor_count; i++) {
        Motor_InterfaceTypeDef *motor = g_motor_manager.motor_list[i];
        Motor_StateTypeDef state;

        // 跳过未初始化/故障电机
        if (motor->GetState(motor, &state) != STATUS_OK) {
            continue;
        }
        if (state.state != MOTOR_STATE_RUNNING) {
            continue;
        }

        // 执行电机周期控制（由适配层实现具体逻辑）
        motor->Control(motor);

        // 故障检测（由适配层实现）
        if (state.fault != MOTOR_FAULT_NONE) {
            Motor_FaultHandle(motor, state.fault);
        }
    }
}

// -------------------------- 私有工具函数 --------------------------
static uint8_t Motor_GetIndex(Motor_InterfaceTypeDef *motor) {
    for (uint8_t i = 0; i < g_motor_manager.motor_count; i++) {
        if (g_motor_manager.motor_list[i] == motor) {
            return i;
        }
    }
    return MAX_MOTOR_NUM;  // 未找到返回无效索引
}