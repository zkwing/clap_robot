#include "bsp_usart.h"

int8_t bsp_usart_init(uart_class_t *puart,
                      UART_HandleTypeDef *huart,
                      bool usart_irq,
                      bool usart_dma_tx_enable,
                      bool usart_dma_rx_enable,
                      uint16_t tx_buffer_size,
                      uint16_t rx_buffer_size)
{
    puart->huart=huart;
    puart->usart_irq = usart_irq;
    puart->usart_dma_tx_enable = usart_dma_tx_enable;
    puart->usart_dma_rx_enable = usart_dma_rx_enable;
    puart->tx_buffer_size = tx_buffer_size;
    puart->rx_buffer_size = rx_buffer_size;


}

int8_t bsp_usart_send(uart_class_t *puart,uint8_t *data,uint16_t size)
{
    if(size==0 || puart==NULL || data == NULL){
        return -1;
    }
    
}
int8_t bsp_usart_dma_send(uart_class_t *puart,uint8_t *data,uint16_t size);
int8_t bsp_usart_rx_to_buffer(uart_class_t *puart);
int8_t bsp_usart_tx_to_buffer(uart_class_t *puart);
int8_t bsp_usart_tx_from_buffer(uart_class_t *puart);