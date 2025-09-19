#include "RingBuffer.h"
#include <string.h> // 只需要这个

/**
 * **********************************************************************************
 * @brief 初始化环形缓冲区
 * @param fp 
 * @param buffer_pool 
 * @param pool_size 
 * @param element_size 
 * @return true 
 * @return false 
 * **********************************************************************************
 */
int8_t ring_buffer_init(ring_buffer_t *fp, void *buffer_pool, uint16_t pool_size, uint16_t element_size)
{
    if(fp == NULL || buffer_pool == NULL || element_size == 0) {
        return 0;
    }
    fp->buffer_pool = buffer_pool;
    fp->element_size = element_size;
    fp->max_buffer_size = pool_size / element_size;
    fp->front = 0;
    fp->rear = 0;
    fp->len = 0;
    return 1;
}

/**
 * **********************************************************************************
 * @brief 获取当前数据长度
 * @param fp 
 * @return uint16_t 
 * **********************************************************************************
 */
uint16_t ring_buffer_get_size(ring_buffer_t *fp)
{
    if(fp == NULL) {
        return 0;
    }
    return fp->len;
}

/**
 * **********************************************************************************
 * @brief 判断队列是否已满
 * @param fp 
 * @return uint8_t 
 * **********************************************************************************
 */
uint8_t ring_buffer_is_full(ring_buffer_t *fp)
{
    if(fp == NULL) return 0;
    return fp->len >= fp->max_buffer_size;
}

/**
 * **********************************************************************************
 * @brief 判断队列是否为空
 * @param fp 
 * @return uint8_t 
 * **********************************************************************************
 */
uint8_t ring_buffer_is_empty(ring_buffer_t *fp)
{
    if(fp == NULL) return 1;
    return fp->len == 0;
}

/**
 * **********************************************************************************
 * @brief 入队一个元素
 * @param fp 
 * @param value 
 * @return true 
 * @return false 
 * **********************************************************************************
 */
int8_t ring_buffer_push(ring_buffer_t *fp, void *value)
{
    if(fp == NULL || value == NULL) return 0;

    memcpy((uint8_t *)fp->buffer_pool + (fp->rear * fp->element_size),
           value, fp->element_size);

    if(ring_buffer_is_full(fp)) {
        fp->front = (fp->front + 1) % fp->max_buffer_size; // 覆盖最旧数据
    } else {
        fp->len++;
    }
    fp->rear = (fp->rear + 1) % fp->max_buffer_size;
    return 1;
}

/**
 * **********************************************************************************
 * @brief 出队一个元素
 * @param fp 
 * @param value 
 * @return true 
 * @return false 
 * **********************************************************************************
 */
int8_t ring_buffer_pop(ring_buffer_t *fp, void *value)
{
    if(fp == NULL || value == NULL || ring_buffer_is_empty(fp)) {
        return 0;
    }
    memcpy(value,
           (uint8_t *)fp->buffer_pool + (fp->front * fp->element_size),
           fp->element_size);

    fp->front = (fp->front + 1) % fp->max_buffer_size;
    fp->len--;
    return 1;
}

/**
 * **********************************************************************************
 * @brief 入队多个元素
 * @param fp 
 * @param value 
 * @param length 
 * @return uint16_t 
 * **********************************************************************************
 */
uint16_t ring_buffer_push_multi(ring_buffer_t *fp, void *value, uint16_t length)
{
    if(fp == NULL || value == NULL || length == 0) return 0;
    uint8_t *u8pval = (uint8_t *)value;
    uint16_t push_len = 0;
    while(length--) {
        if(ring_buffer_push(fp, u8pval)) {
            push_len++;
            u8pval += fp->element_size;
        } else {
            break;
        }
    }
    return push_len;
}

/**
 * **********************************************************************************
 * @brief 出队多个元素
 * @param fp 
 * @param value 
 * @param length 
 * @return uint16_t 
 * **********************************************************************************
 */
uint16_t ring_buffer_pop_multi(ring_buffer_t *fp, void *value, uint16_t length)
{
    if(fp == NULL || value == NULL || length == 0 || ring_buffer_is_empty(fp)) return 0;
    uint16_t pop_len = 0;
    uint8_t *u8pval = (uint8_t *)value;
    while(length--) {
        if(ring_buffer_pop(fp, u8pval)) {
            pop_len++;
            u8pval += fp->element_size;
        } else {
            break;
        }
    }
    return pop_len;
}
