#ifndef _BSP_I2C_SW_C
#define _BSP_I2C_SW_C

/********************************************************************************************
* include:
********************************************************************************************/
#include "bsp_config.h"
#include "bsp_i2c_sw.h"


/********************************************************************************************
* Define:
********************************************************************************************/

static void i2c_sw_start(i2c_sw_class_t *ptr);
static void i2c_sw_stop(i2c_sw_class_t *ptr);
static void i2c_sw_ack(i2c_sw_class_t *ptr);
static void i2c_sw_nack(i2c_sw_class_t *ptr);
static uint8_t i2c_sw_wait_ack(i2c_sw_class_t *ptr);
static void i2c_sw_sendbyte(i2c_sw_class_t *ptr, uint8_t byte);
static uint8_t i2c_sw_readbyte(i2c_sw_class_t *ptr);

static void i2c_pin_set(gpio_class_t *port);
static void i2c_pin_reset(gpio_class_t *port);
static uint8_t i2c_pin_state_get(gpio_class_t *port, uint8_t mode);
/********************************************************************************************
* Datas:
********************************************************************************************/


/********************************************************************************************
* Function:
********************************************************************************************/

/********************************************************************************************
* Function Name  : bsp_i2c_init
* Description    : i2c外设初始化
* Input          :
                  i2c_periph:i2c外设
                  clkspeed  :i2c通信频率
                  addr :i2c设备地址
* Output         : void
* Return         : none
********************************************************************************************/
#pragma clang optimize off
void bsp_i2c_sw_init(i2c_sw_class_t *ptr)
{
    bsp_gpio_init(ptr->scl_port);
    bsp_gpio_init(ptr->sda_port);
    i2c_sw_stop(ptr);

}


uint32_t bsp_i2c_sw_write_buf(i2c_sw_class_t *ptr, uint32_t addr, uint32_t reg, uint8_t *buf, int len)
{

    uint32_t state = I2C_START;
    uint32_t res = I2C_OK;
    volatile int time_count = 0;
    volatile int time_count_max = 10000;
    while(1) {
        switch(state) {
            case I2C_START:
                i2c_sw_start(ptr);
                state = I2C_SEND_ADDRESS;
                break;
            case I2C_SEND_ADDRESS:
                time_count = 0;
                i2c_sw_sendbyte(ptr, addr << 1);
                while(!i2c_sw_wait_ack(ptr)) {
                    if(time_count > time_count_max) {
                        res = I2C_ERR;
                        break;
                    } else {
                        time_count++;
                    }
                }
                if(res == I2C_OK) {
                    state = I2C_SEND_REG;
                }
                break;
            case I2C_SEND_REG:
                time_count = 0;
                i2c_sw_sendbyte(ptr, reg);
                while(!i2c_sw_wait_ack(ptr)) {
                    if(time_count > time_count_max) {
                        res = I2C_ERR;
                        break;
                    } else {
                        time_count++;
                    }
                }
                if(res == I2C_OK) {
                    state = I2C_TRANSMIT_DATA;
                }
                break;
            case I2C_TRANSMIT_DATA:
                for(int j = 0; j < len; j++) {
                    time_count = 0;
                    i2c_sw_sendbyte(ptr, buf[j]);
                    while(!i2c_sw_wait_ack(ptr)) {
                        if(time_count > time_count_max) {
                            res = I2C_ERR;
                            break;
                        } else {
                            time_count++;
                        }
                    }
                    if(res == I2C_ERR) {
                        break;
                    }
                }
                if(res == I2C_OK) {
                    state = I2C_STOP;
                }
                break;
            case I2C_STOP:
                i2c_sw_stop(ptr);
                state = I2C_END;
                break;
            default:
                break;
        }
        if(res == I2C_ERR || state == I2C_END) {
            break;
        }
    }
    return res;
}

uint32_t bsp_i2c_sw_read_buf(i2c_sw_class_t *ptr, uint32_t addr, const uint32_t reg, uint8_t *buf, int len)
{
    i2c_sw_process_enum state = I2C_START;
    uint32_t res = I2C_OK;
    volatile int time_count = 0;
    volatile int time_count_max = 10000;
    uint8_t read_cycle = 0;
    //  //test
    //  i2c_sw_start(ptr);
    //  i2c_sw_sendbyte(ptr,addr<<1 | 0x00);
    //  i2c_sw_wait_ack(ptr);
    //  i2c_sw_sendbyte(ptr,reg);
    //  i2c_sw_wait_ack(ptr);
    //  i2c_sw_start(ptr);
    //  i2c_sw_sendbyte(ptr,addr<<1 | 0x01);
    //  i2c_sw_wait_ack(ptr);
    //
    //  for(int j=0;j<len;j++)
    //  {
    //      *buf=i2c_sw_readbyte(ptr);
    //      if(j!=len-1)
    //      {
    //          i2c_sw_ack(ptr);
    //      }
    //      else
    //      {
    //          i2c_sw_nack(ptr);
    //      }
    //      if(res==I2C_ERR)
    //          break;
    //      buf++;
    //  }
    //  i2c_sw_stop(ptr);
    //
    while(1) {
        switch(state) {
            case I2C_START:
                i2c_sw_start(ptr);
                state = I2C_SEND_ADDRESS;

                break;
            case I2C_SEND_ADDRESS:
                time_count = 0;
                if(read_cycle == 0) {
                    i2c_sw_sendbyte(ptr, addr << 1 | 0x00);
                } else {
                    i2c_sw_sendbyte(ptr, addr << 1 | 0x01);
                }
                while(!i2c_sw_wait_ack(ptr)) {
                    if(time_count > time_count_max) {
                        res = I2C_ERR;
                        break;
                    } else {
                        time_count++;
                    }
                }
                if(res == I2C_OK) {
                    if(read_cycle == 0) {
                        state = I2C_SEND_REG;
                    } else {
                        state = I2C_READ_DATA;
                    }
                }
                break;
            case I2C_SEND_REG:
                time_count = 0;
                i2c_sw_sendbyte(ptr, reg);
                while(!i2c_sw_wait_ack(ptr)) {
                    if(time_count > time_count_max) {
                        res = I2C_ERR;
                        break;
                    } else {
                        time_count++;
                    }
                }
                if(res == I2C_OK) {
                    state = I2C_START;
                    read_cycle = 1;
                }
                break;
            case I2C_READ_DATA:
                for(int j = 0; j < len; j++) {
                    time_count = 0;
                    *buf = i2c_sw_readbyte(ptr);
                    if(j != len - 1) {
                        i2c_sw_ack(ptr);
                    } else {
                        i2c_sw_nack(ptr);
                    }
                    if(res == I2C_ERR) {
                        break;
                    }
                    buf++;
                }
                if(res == I2C_OK) {
                    state = I2C_STOP;
                }
                break;
            case I2C_STOP:
                i2c_sw_stop(ptr);
                state = I2C_END;
                break;
            default:
                break;
        }
        if(res == I2C_ERR || state == I2C_END) {
            break;
        }
    }
    return res;
}


static void i2c_sw_start(i2c_sw_class_t *ptr)
{
    i2c_pin_set(ptr->sda_port);
    i2c_pin_set(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_reset(ptr->sda_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_reset(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
}
static void i2c_sw_stop(i2c_sw_class_t *ptr)
{
    i2c_pin_reset(ptr->sda_port);
    i2c_pin_set(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_set(ptr->sda_port);
}
static void i2c_sw_ack(i2c_sw_class_t *ptr)
{
    i2c_pin_reset(ptr->sda_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_set(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_reset(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_set(ptr->sda_port);
}
static void i2c_sw_nack(i2c_sw_class_t *ptr)
{
    i2c_pin_set(ptr->sda_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_set(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_reset(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
}
static uint8_t i2c_sw_wait_ack(i2c_sw_class_t *ptr)
{
    uint8_t ret;
    i2c_pin_set(ptr->sda_port);
    ptr->delay_ptr(ptr->delay_time);
    i2c_pin_set(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    if(i2c_pin_state_get(ptr->sda_port, 0)) {
        ret = 1;
    } else {
        ret = 0;
    }
    i2c_pin_reset(ptr->scl_port);
    ptr->delay_ptr(ptr->delay_time);
    return ret;
}
static void i2c_sw_sendbyte(i2c_sw_class_t *ptr, uint8_t byte)
{
    uint8_t i;

    for(i = 0; i < 8; i++) {
        if(byte & 0x80) {
            i2c_pin_set(ptr->sda_port);
        } else {
            i2c_pin_reset(ptr->sda_port);
        }
        ptr->delay_ptr(ptr->delay_time);
        i2c_pin_set(ptr->scl_port);
        ptr->delay_ptr(ptr->delay_time);
        i2c_pin_reset(ptr->scl_port);
        if(i == 7) {
            i2c_pin_set(ptr->sda_port);
        }
        byte <<= 1;
        ptr->delay_ptr(ptr->delay_time);
    }
}

static uint8_t i2c_sw_readbyte(i2c_sw_class_t *ptr)
{
    uint8_t i;
    uint8_t value;

    value = 0;
    for(i = 0; i < 8; i++) {
        value <<= 1;
        i2c_pin_set(ptr->scl_port);
        ptr->delay_ptr(ptr->delay_time);
        if(i2c_pin_state_get(ptr->sda_port, 1)) {
            value++;
        }
        i2c_pin_reset(ptr->scl_port);
        ptr->delay_ptr(ptr->delay_time);
    }
    return value;
}
static void i2c_pin_set(gpio_class_t *port)
{
    gpio_bit_set(port->gpio_periph, port->pin);
}
static void i2c_pin_reset(gpio_class_t *port)
{
    gpio_bit_reset(port->gpio_periph, port->pin);
}
static uint8_t i2c_pin_state_get(gpio_class_t *port, uint8_t mode)
{
    if(mode == 0) {
        return gpio_output_bit_get(port->gpio_periph, port->pin);
    } else {
        return gpio_input_bit_get(port->gpio_periph, port->pin);
    }
}
#pragma clang optimize on
#endif
