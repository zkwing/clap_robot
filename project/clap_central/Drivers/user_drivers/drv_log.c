#include "drv_log.h"


void drv_log_init(void)
{
    elog_init();
    /* 设置各等级格式：时间 + 任务名 + 标签 + 内容 */
    elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
    elog_set_fmt(ELOG_LVL_ERROR,  ELOG_FMT_LVL|ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_WARN,   ELOG_FMT_LVL|ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_INFO,   ELOG_FMT_LVL|ELOG_FMT_TAG);
    elog_set_fmt(ELOG_LVL_DEBUG,  ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
    elog_start();
}

void jcom_float_print(uint8_t ch,float data1,float data2,float data3)
{
  uint8_t msg[15];
  msg[0]=0xAA;
  msg[14]=0xBB;
  msg[1]=ch;
  union{
    float d;
    uint8_t int_d[4];
  }f_d;
  f_d.d=data1;
  msg[2]=f_d.int_d[0];
  msg[3]=f_d.int_d[1];
  msg[4]=f_d.int_d[2];
  msg[5]=f_d.int_d[3];

  f_d.d=data2;
  msg[6]=f_d.int_d[0];
  msg[7]=f_d.int_d[1];
  msg[8]=f_d.int_d[2];
  msg[9]=f_d.int_d[3];

  f_d.d=data3;
  msg[10]=f_d.int_d[0];
  msg[11]=f_d.int_d[1];
  msg[12]=f_d.int_d[2];
  msg[13]=f_d.int_d[3];
  bsp_usart_send(&g_debug_usart1,msg,15);
}
