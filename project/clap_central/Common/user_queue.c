#include "user_queue.h"
#include <stdio.h>
#include <string.h>

/*
*********************************************************************************************************
*	函 数 名:   queue_init
*	功能说明: 静态初始化队列
*	形    参： queue 队列对象;
*             queuepool 队列缓存区;
*             pool_size 缓存区大小(单位字节)
*             queue_size 队列元素大小(单位字节)
*             keep_fresh 是否为保持最新模式,true：保持最新 false：默认(存满不能再存)
*	返 回 值: true 初始化成功;false 初始化失败
*********************************************************************************************************
*/
bool Queue_init(struct queue *queue, void *queuepool, uint16_t pool_size,
                   uint16_t queue_size, bool keep_fresh)
{
    
    if (queue == NULL || queuepool == NULL)
        return false;
    queue->keep_fresh = keep_fresh;
    queue->queue_pool = queuepool;
    queue->queue_size = queue_size;
    queue->max_queues = pool_size / queue->queue_size;
    queue->front = 0;
    queue->rear = 0;
    queue->len = 0;
    return true;
}

/*
*********************************************************************************************************
*	函 数 名: queue_detach
*	功能说明: 会使缓存区脱离与队列的关联。必须为静态方式创建的队列对象。
*	形    参：queue 要脱离的队列对象

*	返 回 值: 
*              true 脱离成功
*              false 脱离失败
*********************************************************************************************************
*/
bool Queue_detach(struct queue *queue)
{

    if (queue == NULL)
        return false;
    queue->queue_pool = NULL;
    queue->front = 0;
    queue->rear = 0;
    queue->len = 0;
    return true;
}


/*
*********************************************************************************************************
*	函 数 名: queue_create
*	功能说明: 动态创建队列
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
struct queue* Queue_create(uint16_t queue_size, uint16_t max_queues,
                                 bool keep_fresh)
{
    
    struct queue *queue;
    if ((queue = malloc(sizeof(struct queue))) == NULL)
        return NULL;
    queue->queue_size = queue_size;
    queue->max_queues = max_queues;
    queue->queue_pool = malloc(queue->queue_size * queue->max_queues);
    if (queue->queue_pool == NULL)
    {
        free(queue);
        return NULL;
    }
    queue->keep_fresh = keep_fresh;
    queue->front = 0;
    queue->rear = 0;
    queue->len = 0;
    return queue;
}

/*
*********************************************************************************************************
*	函 数 名: queue_delete
*	功能说明: 动态删除队列
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_delete(struct queue *queue)
{
    
    if (queue == NULL)
        return false;
    free(queue->queue_pool);
    free(queue);
    return true;
}


/*
*********************************************************************************************************
*	函 数 名: queue_clean
*	功能说明: 清空队列
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_clean(struct queue *queue)
{
    
    if (queue == NULL)
        return false;
    queue->front = 0;
    queue->rear = 0;
    queue->len = 0;
    return true;
}

/*
*********************************************************************************************************
*	函 数 名: queue_curr_len
*	功能说明: 查询队列当前数据长度
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t Queue_curr_len(struct queue *queue)
{
    
    if (queue == NULL)
        return false;
    return (queue->len);
}

/*
*********************************************************************************************************
*	函 数 名: queue_empty
*	功能说明: 判断队列是否为空
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_empty(struct queue *queue)
{
    
    if (queue->len == 0)
        return true;
    return false;
}

/*
*********************************************************************************************************
*	函 数 名: queue_full
*	功能说明: 判断队列是否已满
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_full(struct queue *queue)
{
    
    if (queue->len >= queue->max_queues)
        return true;
    return false;
}

/*
*********************************************************************************************************
*	函 数 名: queue_push
*	功能说明: 向队列压入(入队)1个元素数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_push(struct queue *queue, void *val)
{
    
    
    if (Queue_full(queue))
    {
        if (queue->keep_fresh == true)
        {
            memcpy((uint8_t *)queue->queue_pool + (queue->rear * queue->queue_size),
                   (uint8_t *)val, queue->queue_size);

            queue->rear = (queue->rear + 1) % queue->max_queues;
            queue->front = (queue->front + 1) % queue->max_queues;
            queue->len = queue->max_queues;
            return true;
        }
        return false;
    }
    else
    {
        memcpy((uint8_t *)queue->queue_pool + (queue->rear * queue->queue_size),
               (uint8_t *)val, queue->queue_size);

        queue->rear = (queue->rear + 1) % queue->max_queues;
        queue->len++;
    }
    return true;
}

/*
*********************************************************************************************************
*	函 数 名: queue_pop
*	功能说明: 从队列弹出(出队)1个元素数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_pop(struct queue *queue, void *pval)
{
    
    
    if (Queue_empty(queue))
    {
        return false;
    }
    else
    {
        memcpy((uint8_t *)pval,
               (uint8_t *)queue->queue_pool + (queue->front * queue->queue_size),
               queue->queue_size);

        queue->front = (queue->front + 1) % queue->max_queues;
        queue->len--;
    }
    return true;
}

/*
*********************************************************************************************************
*	函 数 名: queue_push_multi
*	功能说明: 向队列压入(入队)多个元素数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t Queue_push_multi(struct queue *queue, void *pval, uint16_t len)
{
    
    
    uint8_t *u8pval = pval;
    uint16_t push_len = 0;
    while (len-- && Queue_push(queue, u8pval) == true)
    {
        push_len++;
        u8pval += queue->queue_size;
    }

    return push_len;
}

/*
*********************************************************************************************************
*	函 数 名: queue_pop_multi
*	功能说明: 从队列弹出(出队)多个元素数据
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t Queue_pop_multi(struct queue *queue, void *pval, uint16_t len)
{
    
    
    uint16_t pop_len = 0;
    uint8_t *u8pval = pval;
    if (Queue_empty(queue) == true)
        return false;
    while (len-- && Queue_pop(queue, u8pval) == true)
    {
        pop_len++;
        u8pval += queue->queue_size;
    }
    return pop_len;
}

/*
*********************************************************************************************************
*	函 数 名: queue_peep
*	功能说明: 从队列中读取一个元素(不从队列中删除)
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_peep(struct queue *queue, void *pval)
{
    
    
    if (Queue_empty(queue))
    {
        return false;
    }
    else
    {
        memcpy((uint8_t *)pval,
               (uint8_t *)queue->queue_pool + (queue->front * queue->queue_size),
               queue->queue_size);
    }
    return true;
}

/*
*********************************************************************************************************
*	函 数 名: queue_remove
*	功能说明: 移除一个元素
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
bool Queue_remove(struct queue *queue)
{
    if (Queue_empty(queue))
    {
        return true;
    }
    queue->front = (queue->front + 1) % queue->max_queues;
    queue->len--;

    return true;
}

