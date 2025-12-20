#ifndef MOTOR_CORE_H
#define MOTOR_CORE_H

/********************************************************************************************
* include:
********************************************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"
/********************************************************************************************
* Define:
********************************************************************************************/
// 操作状态码（所有接口统一返回）
typedef enum {
    MOTOR_STATUS_OK = 0,      // 操作成功
    MOTOR_STATUS_ERROR,       // 通用错误
    MOTOR_STATUS_INVALID_PARAM, // 参数无效
    MOTOR_STATUS_UNSUPPORTED, // 不支持的操作
    MOTOR_STATUS_FAULT        // 电机故障
} Motor_Status_Def;

typedef enum {
    MOTOR_TYPE_DC = 0,        // 直流有刷电机
    MOTOR_TYPE_BLDC,          // 无刷电机
    MOTOR_TYPE_MAX            // 电机类型数量（用于校验）
} Motor_Type_Def;

typedef enum {
    CTRL_POSITION = 0,        // 位置控制
    CTRL_SPEED,               // 速度控制
    CTRL_CURRENT,             // 电流控制
    CTRL_MAX                  // 控制类型数量（用于校验）
} Motor_Ctrl_Type_Def;

// 电机故障状态码
typedef enum {
    MOTOR_FAULT_NONE = 0,           // 无故障
    MOTOR_FAULT_OVER_CURRENT,       // 过流故障
    MOTOR_FAULT_OVER_VOLTAGE,       // 过压故障
    MOTOR_FAULT_OVER_TEMPERATURE,   // 过温故障
    MOTOR_FAULT_MAX                 // 故障数量（用于校验）
} Motor_Fault_Def;

typedef struct {
    Motor_Status_Def (*Init)(void *handle);                         // 初始化函数
    Motor_Status_Def (*Start)(void *handle);                        // 启动函数
    Motor_Status_Def (*Stop)(void *handle);                         // 停止函数
    Motor_Status_Def (*SetTarget)(void *handle, float target);      // 设置目标
    Motor_Status_Def (*GetState)(void *handle);                     // 获取状态
    Motor_Status_Def (*ResetFault)(void *handle);                   // 故障复位
} Motor_OpsDef;

typedef char* Motor_NameDef;
typedef struct {
    Motor_Type_Def type;                // 电机类型
    Motor_Ctrl_Type_Def ctrl_type;      // 控制类型
    Motor_NameDef name;                 // 电机名称
    Motor_OpsDef *ops;                  // 操作函数指针
    float target;                       // 目标值
    float feedback;                     // 当前值
    float max_target;                   // 最大目标值
    float min_target;                   // 最小目标值
    int16_t temperature;                // 电机温度
    void *pri_data;                     // 适配器数据
} Motor_Handle_Def;

/********************************************************************************************
* Datas:
********************************************************************************************/
#define MAX_MOTOR_NUM 10


/********************************************************************************************
* Function:
********************************************************************************************/
extern Motor_Handle_Def* Motor_Find_Device(Motor_NameDef name);
extern Motor_Status_Def Motor_Init(Motor_Handle_Def *motor_handle);
extern Motor_Status_Def Motor_Set_Target(Motor_Handle_Def *motor_handle,float target);
extern Motor_Status_Def Motor_Get_State(Motor_Handle_Def *motor_handle);
extern Motor_Status_Def Motor_Start(Motor_Handle_Def *motor_handle);
extern Motor_Status_Def Motor_Stop(Motor_Handle_Def *motor_handle);
extern Motor_Status_Def Motor_Reset_Fault(Motor_Handle_Def *motor_handle);


#endif  // MOTOR_CORE_H