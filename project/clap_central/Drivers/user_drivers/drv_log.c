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
