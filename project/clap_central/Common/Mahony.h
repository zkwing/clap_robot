#ifndef _MAHONY_H
#define _MAHONY_H

#include <stdint.h>

/* 状态结构体 */
typedef struct {
    float q0, q1, q2, q3;
    float integralFBx, integralFBy, integralFBz;
    float kp, ki;
} mahony_t;

/* 生命周期 */
void mahony_init(mahony_t* m, float kp, float ki);
void mahony_update(mahony_t* m,
                   float gx, float gy, float gz, /* rad/s */
                   float ax, float ay, float az, /* g */
                   float mx, float my, float mz, /* 任意，内部归一 */
                   float dt);                    /* s */

/* 辅助：四元数转欧拉角（rad） */
void mahony_get_euler(const mahony_t* m,
                      float* roll,
                      float* pitch,
                      float* yaw);

#endif