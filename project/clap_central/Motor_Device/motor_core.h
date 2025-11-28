#ifndef MOTOR_CORE_H
#define MOTOR_CORE_H

#include "motor_interface.h"  // 依赖抽象接口，不依赖具体实现
#include <stdint.h>
// -------------------------- 全局配置 --------------------------
#define MAX_MOTOR_NUM         8       // 最大支持电机数量
#define MOTOR_CONTROL_PERIOD  10      // 控制周期（ms）

// -------------------------- 枚举定义 --------------------------
// 电机运行状态
typedef enum {
    MOTOR_STATE_UNINIT = 0,    // 未初始化
    MOTOR_STATE_IDLE,          // 空闲
    MOTOR_STATE_RUNNING,       // 运行中
    MOTOR_STATE_FAULT,         // 故障
    MOTOR_STATE_STOPPED        // 已停止
} Motor_StateEnum;

// 电机故障类型
typedef enum {
    MOTOR_FAULT_NONE = 0,
    MOTOR_FAULT_OVER_CURRENT,  // 过流
    MOTOR_FAULT_OVER_VOLTAGE,  // 过压
    MOTOR_FAULT_BLOCKED,       // 堵转
    MOTOR_FAULT_TIMEOUT        // 控制超时
} Motor_FaultEnum;

// 状态码定义
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR,
    STATUS_BUSY,
    STATUS_INVALID_PARAM
} StatusTypeDef;

// -------------------------- 结构体定义 --------------------------
// 电机状态结构体（供上层查询）
typedef struct {
    Motor_StateEnum state;        // 运行状态
    Motor_FaultEnum fault;        // 故障状态
    int32_t target;               // 目标值（速度/角度/位置）
    int32_t current;              // 当前值
    uint32_t runtime;             // 运行时长（ms）
    uint32_t reserved[2];         // 预留扩展字段
} Motor_StateTypeDef;

// 电机管理器（单例）
typedef struct {
    Motor_InterfaceTypeDef *motor_list[MAX_MOTOR_NUM];  // 电机实例列表
    uint8_t motor_count;                                // 已注册电机数量
    uint32_t tick_count;                                // 系统滴答计数（用于调度）
} Motor_ManagerTypeDef;

// 故障回调函数类型
typedef void (*Motor_FaultCallbackTypeDef)(Motor_InterfaceTypeDef *motor, Motor_FaultEnum fault);

// -------------------------- 核心API声明 --------------------------
// 初始化电机管理器
StatusTypeDef Motor_ManagerInit(void);

// 注册/注销电机
StatusTypeDef Motor_Register(Motor_InterfaceTypeDef *motor);
StatusTypeDef Motor_Unregister(Motor_InterfaceTypeDef *motor);

// 电机控制API
StatusTypeDef Motor_Start(Motor_InterfaceTypeDef *motor);
StatusTypeDef Motor_Stop(Motor_InterfaceTypeDef *motor);
StatusTypeDef Motor_SetTarget(Motor_InterfaceTypeDef *motor, int32_t target);
StatusTypeDef Motor_GetState(Motor_InterfaceTypeDef *motor, Motor_StateTypeDef *state);

// 故障管理
void Motor_RegisterFaultCallback(Motor_FaultCallbackTypeDef callback);
void Motor_FaultHandle(Motor_InterfaceTypeDef *motor, Motor_FaultEnum fault);

// 控制调度任务（定时器中断/主循环调用）
void Motor_ControlSchedulerTask(void);

// 全局电机管理器实例（外部可访问）
extern Motor_ManagerTypeDef g_motor_manager;

#endif  // MOTOR_CORE_H