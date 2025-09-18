#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "main.h"
typedef struct 
{
    UART_HandleTypeDef *huart;

    bool usart_irq;
    bool usart_dma_tx_enable;
    bool usart_dma_rx_enable;

    queue_t tx_buffer;
    queue_t rx_buffer;

    uint16_t tx_buffer_size;
    uint16_t rx_buffer_size;
}uart_class_t;

int8_t bsp_usart_init(uart_class_t *puart,
                      UART_HandleTypeDef *huart,
                      bool usart_irq,
                      bool usart_dma_tx_enable,
                      bool usart_dma_rx_enable,
                      uint16_t tx_buffer_size,
                      uint16_t rx_buffer_size);

int8_t bsp_usart_send(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_dma_send(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_rx_to_buffer(uart_class_t *puart);
int8_t bsp_usart_tx_to_buffer(uart_class_t *puart);
int8_t bsp_usart_tx_from_buffer(uart_class_t *puart);

#endif