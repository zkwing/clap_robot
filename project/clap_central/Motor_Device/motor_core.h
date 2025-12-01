#ifndef MOTOR_CORE_H
#define MOTOR_CORE_H

/********************************************************************************************
* include:
********************************************************************************************/
#include "stdint.h"


/********************************************************************************************
* Define:
********************************************************************************************/
typedef enum {
    MOTOR_TYPE_DC = 0,        // 直流有刷电机
    MOTOR_TYPE_BLDC,          // 无刷电机
    MOTOR_TYPE_MAX            // 电机类型数量（用于校验）
} Motor_Type_Def;

typedef enum {
    CTRL_POSITION = 0,        // 位置控制
    CTRL_SPEED,               // 速度控制
    CTRL_CURRENT,             // 电流控制
} Motor_Ctrl_Type_Def;

// 操作状态码（所有接口统一返回）
typedef enum {
    MOTOR_STATUS_OK = 0,      // 操作成功
    MOTOR_STATUS_ERROR,       // 通用错误
    MOTOR_STATUS_INVALID_PARAM, // 参数无效
    MOTOR_STATUS_UNSUPPORTED, // 不支持的操作
    MOTOR_STATUS_FAULT        // 电机故障
} Motor_Status_Def;

typedef struct {
    Motor_Type_Def type;                // 电机类型
    Motor_Ctrl_Type_Def ctrl_type;      // 控制类型
    int32_t target;                     // 目标值
    int32_t feedback;                   // 当前值

} Motor_Handle_Def;

/********************************************************************************************
* Datas:
********************************************************************************************/



/********************************************************************************************
* Function:
********************************************************************************************/
Motor_Status_Def Motor_Init(Motor_Handle_Def *motor_handle);
Motor_Status_Def Motor_setTarget(Motor_Handle_Def *motor_handle,int32_t target);
Motor_Status_Def Motor_getState(Motor_Handle_Def *motor_handle);
Motor_Status_Def Motor_Start(Motor_Handle_Def *motor_handle);
Motor_Status_Def Motor_Stop(Motor_Handle_Def *motor_handle);
Motor_Status_Def Motor_Reset(Motor_Handle_Def *motor_handle);


#endif  // MOTOR_CORE_H