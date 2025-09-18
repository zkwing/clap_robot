#ifndef _USER_QUEUE_H_
#define _USER_QUEUE_H_

#include "stm32h7xx_hal.h"
#include "stdbool.h"
#include "stdio.h"
#include<stdlib.h>
#include<stdint.h>




struct queue
{
    bool keep_fresh;
	void *queue_pool;
	uint16_t queue_size;
	uint16_t max_queues;
    uint16_t front;
    uint16_t rear;
    uint16_t len;
};
typedef struct queue *queue_t;


bool Queue_init(struct queue *queue, void *queuepool, uint16_t pool_size,uint16_t queue_size, bool keep_fresh);
bool Queue_detach(struct queue *queue);
struct queue *Queue_create(uint16_t queue_size, uint16_t max_queues,bool keep_fresh);
bool Queue_delete(struct queue *queue);
bool Queue_clean(struct queue *queue);
uint16_t Queue_curr_len(struct queue *queue);
bool Queue_empty(struct queue *queue);
bool Queue_full(struct queue *queue);
bool Queue_push(struct queue *queue, void *val);
bool Queue_pop(struct queue *queue, void *pval);
uint16_t Queue_push_multi(struct queue *queue, void *pval, uint16_t len);
uint16_t Queue_pop_multi(struct queue *queue, void *pval, uint16_t len);
bool Queue_peep(struct queue *queue, void *pval);
bool Queue_remove(struct queue *queue);


#endif
