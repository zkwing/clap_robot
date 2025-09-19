#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "main.h"
typedef struct 
{
    UART_HandleTypeDef *huart;
    DMA_HandleTypeDef *hdma_usart_tx;
    DMA_HandleTypeDef *hdma_usart_rx;

    uint32_t usart_irq;
    bool usart_dma_tx_enable;
    bool usart_dma_rx_enable;

    ring_buffer_t tx_ringbuffer;
    ring_buffer_t rx_ringbuffer;

    uint16_t tx_ringbuffer_size;
    uint16_t rx_ringbuffer_size;

    uint8_t *tx_dmabuffer;
    uint8_t *rx_dmabuffer;
    uint16_t tx_dmabuffer_size;
    uint16_t rx_dmabuffer_size;
}uart_class_t;

int8_t bsp_usart_init(uart_class_t *puart,
                      UART_HandleTypeDef *huart,
                      DMA_HandleTypeDef *hdma_usart_tx,
                      DMA_HandleTypeDef *hdma_usart_rx,
                      uint32_t usart_irq,
                      bool usart_dma_tx_enable,
                      bool usart_dma_rx_enable,
                      uint16_t tx_ringbuffer_size,
                      uint16_t rx_ringbuffer_size,
                      uint16_t tx_dmabuffer_size,
                      uint16_t rx_dmabuffer_size);

int8_t bsp_usart_send(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_dma_send(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_send_to_buffer(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_send_from_buffer(uart_class_t *puart);
int8_t bsp_usart_dma_send_from_buffer(uart_class_t *puart);

int8_t bsp_usart_rx_to_buffer(uart_class_t *puart);
int8_t bsp_usart_rx_dma_to_buffer(uart_class_t *puart);

#endif