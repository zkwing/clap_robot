#include "bsp_init.h"
#include "usart.h"

uart_class_t g_debug_usart1;
int8_t bsp_init(void)
{
  bsp_usart_init(&g_debug_usart1,&huart1,&hdma_usart1_tx,&hdma_usart1_rx,UART_IT_IDLE,true,true,128,128,128,128);
  return 0;
}

