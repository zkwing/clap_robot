
#ifndef _BSP_I2C_SW_H
#define _BSP_I2C_SW_H


/********************************************************************************************
* include:
********************************************************************************************/
#include "bsp_config.h"
#include "stdint.h"
#include "drv_delay.h"
/********************************************************************************************
* Define:
********************************************************************************************/
typedef struct {
    gpio_class_t *scl_port;
    gpio_class_t *sda_port;
    sw_delay_ptr delay_ptr;
    uint16_t delay_time;
} i2c_sw_class_t;

typedef enum {
    I2C_START = 0,
    I2C_SEND_ADDRESS,
    I2C_SEND_REG,
    I2C_TRANSMIT_DATA,
    I2C_READ_DATA,
    I2C_STOP,
    I2C_END,
} i2c_sw_process_enum;


#define I2C_OK 0x00
#define I2C_ERR 0x01



/********************************************************************************************
* Datas:
********************************************************************************************/



/********************************************************************************************
* Function:
********************************************************************************************/

//void bsp_i2c_test_init(uint32_t i2c_periph,uint32_t clkspeed,uint32_t addr);
//uint32_t i2c_write_buf(uint32_t i2c_periph,uint8_t addr,uint32_t reg,uint8_t *buf,int len);
//uint32_t i2c_read_buf(uint32_t i2c_periph,uint8_t addr,uint32_t reg,uint8_t *buf,int len);

void bsp_i2c_sw_init(i2c_sw_class_t *ptr);
uint32_t bsp_i2c_sw_write_buf(i2c_sw_class_t *ptr, uint32_t addr, uint32_t reg, uint8_t *buf, int len);
uint32_t bsp_i2c_sw_read_buf(i2c_sw_class_t *ptr, uint32_t addr, uint32_t reg, uint8_t *buf, int len);

#endif
