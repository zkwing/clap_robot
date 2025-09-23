#include "bsp_init.h"
#include "usart.h"
#include "i2c.h"

uart_class_t g_debug_usart1;
i2c_hw_class_t g_mag_sensor_hw_i2c;
i2c_hw_class_t g_acc_sensor_hw_i2c;
i2c_hw_class_t g_gyro_sensor_hw_i2c;

int8_t bsp_init(void)
{
  bsp_usart_init(&g_debug_usart1,&huart1,&hdma_usart1_tx,&hdma_usart1_rx,UART_IT_IDLE,true,true,128,128,128,128);
  bsp_i2c_hw_init(&g_mag_sensor_hw_i2c,&hi2c1,0x1E); // hmc 5883L
  bsp_i2c_hw_init(&g_acc_sensor_hw_i2c,&hi2c1,0x1D); // ADXL345
  bsp_i2c_hw_init(&g_gyro_sensor_hw_i2c,&hi2c1,0x68);// ITG-3205
  return 0;
}

