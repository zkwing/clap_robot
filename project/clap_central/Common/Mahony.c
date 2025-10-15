#include "mahony.h"
#include <math.h>

void mahony_init(mahony_t* m, float kp, float ki)
{
    m->q0 = 1.0f;
    m->q1 = m->q2 = m->q3 = 0.0f;
    m->integralFBx = m->integralFBy = m->integralFBz = 0.0f;
    m->kp = kp;
    m->ki = ki;
}

void mahony_update(mahony_t* m,
                   float gx, float gy, float gz,
                   float ax, float ay, float az,
                   float mx, float my, float mz,
                   float dt)
{
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez;

    /* 加速度归一化 */
    norm = sqrtf(ax*ax + ay*ay + az*az);
    if (norm == 0.0f) return;
    ax /= norm; ay /= norm; az /= norm;

    /* 磁力计归一化 */
//    norm = sqrtf(mx*mx + my*my + mz*mz);
//    if (norm == 0.0f) return;
//    mx /= norm; my /= norm; mz /= norm;
    /* 陀螺仪归一化 */
    norm = sqrtf(gx*gx + gy*gy + gz*gz);
    if (norm == 0.0f) return;
    gx /= norm; gy /= norm; gz /= norm;

    /* 把磁力计转到世界系，估算磁北方向 */
    hx = 2.0f*(mx*(0.5f - m->q2*m->q2 - m->q3*m->q3) +
               my*(m->q1*m->q2 - m->q0*m->q3) +   
               mz*(m->q1*m->q3 + m->q0*m->q2));
    hy = 2.0f*(mx*(m->q1*m->q2 + m->q0*m->q3) +
               my*(0.5f - m->q1*m->q1 - m->q3*m->q3) +
               mz*(m->q2*m->q3 - m->q0*m->q1)); 
    hz = 2.0f*(mx*(m->q1*m->q3 - m->q0*m->q2) +
               my*(m->q2*m->q3 + m->q0*m->q1) +
               mz*(0.5f - m->q1*m->q1 - m->q2*m->q2)); 
    bx = sqrtf(hx*hx + hy*hy);
    bz = hz;
 
    /* 期望方向（世界系） */
    vx = 2.0f*(m->q1*m->q3 - m->q0*m->q2);
    vy = 2.0f*(m->q0*m->q1 + m->q2*m->q3);
    vz = m->q0*m->q0 - m->q1*m->q1 - m->q2*m->q2 + m->q3*m->q3;

    wx = 2.0f*bx*(0.5f - m->q2*m->q2 - m->q3*m->q3) + 2.0f*bz*(m->q1*m->q2 - m->q0*m->q3);
    wy = 2.0f*bx*(m->q1*m->q2 + m->q0*m->q3) + 2.0f*bz*(m->q0*m->q1 + m->q2*m->q3);
    wz = 2.0f*bx*(m->q1*m->q3 - m->q0*m->q2) + 2.0f*bz*(0.5f - m->q1*m->q1 - m->q2*m->q2);

    /* 误差向量 */
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    /* PI 补偿 */
    if (m->ki > 0.0f) {
        m->integralFBx += ex;
        m->integralFBy += ey;
        m->integralFBz += ez;
        // m->integralFBx += m->ki * ex * dt;
        // m->integralFBy += m->ki * ey * dt;
        // m->integralFBz += m->ki * ez * dt;
        // gx += m->integralFBx;
        // gy += m->integralFBy;
        // gz += m->integralFBz;
    }else{
        m->integralFBx = 0.0f;
        m->integralFBy = 0.0f;
        m->integralFBz = 0.0f;
    }
    gx =gx + m->kp * ex + m->ki * m->integralFBx;
    gy =gy + m->kp * ey + m->ki * m->integralFBy; 
    gz =gz + m->kp * ez + m->ki * m->integralFBz;

    /* 一阶积分更新四元数 */
    gx *= 0.5f * dt; gy *= 0.5f * dt; gz *= 0.5f * dt;
    float qa = m->q0, qb = m->q1, qc = m->q2;
    m->q0 += (-qb*gx - qc*gy - m->q3*gz);
    m->q1 += ( qa*gx + qc*gz - m->q3*gy);
    m->q2 += ( qa*gy - qb*gz + m->q3*gx);
    m->q3 += ( qa*gz + qb*gy - qc*gx);

    /* 归一化 */
    norm = sqrtf(m->q0*m->q0 + m->q1*m->q1 + m->q2*m->q2 + m->q3*m->q3);
    m->q0 /= norm; m->q1 /= norm; m->q2 /= norm; m->q3 /= norm;
}

void mahony_get_euler(const mahony_t* m,
                      float* roll,
                      float* pitch,
                      float* yaw)
{
    *roll  = atan2f(+2.0f*(m->q0*m->q1 + m->q2*m->q3),
                    +1.0f - 2.0f*(m->q1*m->q1 + m->q2*m->q2));
    *pitch = asinf (+2.0f*(m->q0*m->q2 - m->q3*m->q1));
    *yaw   = atan2f(+2.0f*(m->q0*m->q3 + m->q1*m->q2),
                    +1.0f - 2.0f*(m->q2*m->q2 + m->q3*m->q3));
}
