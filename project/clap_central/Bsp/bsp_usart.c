#include "bsp_usart.h"

int8_t bsp_usart_init(uart_class_t *puart,
                      UART_HandleTypeDef *huart,
                      DMA_HandleTypeDef *dma_tx,
                      DMA_HandleTypeDef *dma_rx,
                      bool usart_irq,
                      bool usart_dma_tx_enable,
                      bool usart_dma_rx_enable,
                      uint16_t tx_ringbuffer_size,
                      uint16_t rx_ringbuffer_size,
                      uint16_t tx_dmabuffer_size,
                      uint16_t rx_dmabuffer_size)
{
    // init uart interface
    puart->huart=huart;
    puart->hdma_usart_tx = dma_tx;
    puart->hdma_usart_rx = dma_rx;
    puart->usart_irq = usart_irq;
    puart->usart_dma_tx_enable = usart_dma_tx_enable;
    puart->usart_dma_rx_enable = usart_dma_rx_enable;
    puart->tx_ringbuffer_size = tx_ringbuffer_size;
    puart->rx_ringbuffer_size = rx_ringbuffer_size;
    puart->tx_dmabuffer_size = tx_dmabuffer_size;
    puart->rx_dmabuffer_size = rx_dmabuffer_size;
    // init uart dma interface
    if(puart->usart_dma_tx_enable){
        
    }
    if(puart->usart_dma_rx_enable){
        
    }
    if(puart->usart_irq){
        
    }
    // init uart
    if(tx_ringbuffer_size>0){
        ring_buffer_init(&puart->tx_ringbuffer, malloc(puart->tx_ringbuffer_size), puart->tx_ringbuffer_size, sizeof(uint8_t));
    }
    if(rx_ringbuffer_size>0){
        ring_buffer_init(&puart->rx_ringbuffer, malloc(puart->rx_ringbuffer_size), puart->rx_ringbuffer_size, sizeof(uint8_t));
    }
    return 0;

}

int8_t bsp_usart_send(uart_class_t *puart,uint8_t *data,uint16_t size)
{
    if(size==0 || puart==NULL || data == NULL){
        return -1;
    }
    int8_t ret = 0;
    for(uint16_t i=0;i<size;i++){
        ret=HAL_USART_Transmit(puart->huart,data[i],1,1);
        if(ret!=0){
            return ret;
        }
    }
    return ret;
}
int8_t bsp_usart_dma_send(uart_class_t *puart,uint8_t *data,uint16_t size)
{
    HAL_UART_Transmit_DMA(puart->huart,data,size);
}

int8_t bsp_usart_send_to_buffer(uart_class_t *puart,uint8_t *data,uint16_t size)
{
    if(puart == NULL || data == NULL || puart->tx_ringbuffer.buffer_pool == NULL) {
        return -1;
    }

    ring_buffer_push_multi(&puart->tx_ringbuffer, data, size);

    return 0;
}
int8_t bsp_usart_send_from_buffer(uart_class_t *puart)
{
    int8_t ret = 0;
    if(puart == NULL || puart->tx_ringbuffer.buffer_pool == NULL) {
        return -1;
    }

    uint8_t data;
    if(puart->tx_ringbuffer_size > 0) {
        ret = ring_buffer_pop_multi(&puart->tx_ringbuffer, &data, 1);
        if(ret != 1){
            return -1;
        }
    }
    ret = HAL_USART_Transmit(puart->huart, data);
    if(ret != 0) {
        return ret;
    }
    return 0;
}

int8_t bsp_usart_rx_to_buffer(uart_class_t *puart)
{
    if(puart == NULL || puart->rx_ringbuffer.buffer_pool == NULL) {
        return -1;
    }

    uint8_t data;
    HAL_UART_Receive(puart->huart, &data, 1, 1);

    ring_buffer_push_multi(&puart->rx_ringbuffer, data, 1);

    return 0;
}

int8_t bsp_usart_rx_dma_to_buffer(uart_class_t *puart)
{
    if(puart == NULL || puart->rx_ringbuffer.buffer_pool == NULL) {
        return -1;
    }
    HAL_UART_DMAStop(puart->huart);
    uint16_t rev_len =  puart->rx_dmabuffer_size-__HAL_DMA_GET_COUNTER(&puart->hdma_usart_rx);
    if(puart->rx_ringbuffer_size > 0) {
        ring_buffer_push_multi(&puart->rx_ringbuffer, puart->rx_dmabuffer, rev_len);
    }
    HAL_UART_Receive_DMA(puart->huart, puart->rx_dmabuffer, puart->rx_dmabuffer_size);
    return 0;
}