#ifndef _RING_BUFFER_H
#define _RING_BUFFER_H
#include <stdint.h>

typedef struct {
    void *buffer_pool;
    uint16_t element_size;
    uint16_t max_buffer_size;
    uint32_t front; // 读指针
    uint32_t rear;  // 写指针
    uint32_t len;
} ring_buffer_t;

int8_t ring_buffer_init(ring_buffer_t *fp, void *buffer_pool, uint16_t pool_size, uint16_t element_size);

uint16_t ring_buffer_get_size(ring_buffer_t *fp);
uint8_t ring_buffer_is_full(ring_buffer_t *fp);
uint8_t ring_buffer_is_empty(ring_buffer_t *fp);

int8_t ring_buffer_push(ring_buffer_t *fp, void *value);
int8_t ring_buffer_pop(ring_buffer_t *fp, void *value);

uint16_t ring_buffer_push_multi(ring_buffer_t *fp, void *value, uint16_t length);
uint16_t ring_buffer_pop_multi(ring_buffer_t *fp, void *value, uint16_t length);

#endif
