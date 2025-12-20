#include "motor_core.h"
#include "motor_config.h"


static bool Motor_CheckHandle(Motor_Handle_Def *handle) {
    if (handle == NULL || handle->type >= MOTOR_TYPE_MAX || handle->ctrl_type >= CTRL_MAX) return false;
    return true;
}

Motor_Handle_Def* Motor_Find_Device(Motor_NameDef name)
{
    //TODO:根据name查找设备
    for(uint8_t i=0;i<MAX_MOTOR_NUM;i++){
        if(strcmp(motor_list[i].name,name) == 0){
            return &motor_list[i];
        }
    }
    return NULL;
}

Motor_Status_Def Motor_Init(Motor_Handle_Def *motor_handle)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    return motor_handle->ops->Init(motor_handle);
}

Motor_Status_Def Motor_Set_Target(Motor_Handle_Def *motor_handle,float target)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    if(target < motor_handle->min_target || target > motor_handle->max_target)
        return MOTOR_STATUS_UNSUPPORTED;
    return motor_handle->ops->SetTarget(motor_handle,target);

}

Motor_Status_Def Motor_Get_State(Motor_Handle_Def *motor_handle)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    return motor_handle->ops->GetState(motor_handle);
}

Motor_Status_Def Motor_Start(Motor_Handle_Def *motor_handle)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    return motor_handle->ops->Start(motor_handle);
}

Motor_Status_Def Motor_Stop(Motor_Handle_Def *motor_handle)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    return motor_handle->ops->Stop(motor_handle);
}

Motor_Status_Def Motor_Reset_Fault(Motor_Handle_Def *motor_handle)
{
    if(!Motor_CheckHandle(motor_handle))return MOTOR_STATUS_INVALID_PARAM;
    return motor_handle->ops->ResetFault(motor_handle);
}