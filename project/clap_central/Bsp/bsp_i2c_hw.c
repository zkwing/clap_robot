#ifndef _BSP_I2C_TEST_C
#define _BSP_I2C_TEST_C

/********************************************************************************************
* include:
********************************************************************************************/
#include "bsp_config.h"
#include "bsp_i2c_hw.h"


/********************************************************************************************
* Define:
********************************************************************************************/


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
void bsp_i2c_hw_init(i2c_hw_class_t *ptr, uint32_t clkspeed, uint32_t addr)
{
    i2c_deinit(ptr->i2c_periph);
    if(ptr->i2c_periph == I2C0) {

        rcu_periph_clock_enable(RCU_AF);
        rcu_periph_clock_enable(RCU_I2C0);
        bsp_gpio_init(ptr->scl_port);
        bsp_gpio_init(ptr->sda_port);
    } else if(ptr->i2c_periph == I2C1) {
        rcu_periph_clock_enable(RCU_I2C1);
        bsp_gpio_init(ptr->scl_port);
        bsp_gpio_init(ptr->sda_port);
    }





    /* I2C clock configure */
    i2c_clock_config(ptr->i2c_periph, clkspeed, I2C_DTCY_2);//I2C_DTCY_16_9
    /* I2C address configure */
    i2c_mode_addr_config(ptr->i2c_periph, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, addr);
    /* enable I2C0 */
    i2c_enable(ptr->i2c_periph);
    /* enable acknowledge */
    i2c_ack_config(ptr->i2c_periph, I2C_ACK_ENABLE);


}

uint32_t bsp_i2c_hw_write_buf(i2c_hw_class_t *ptr, uint8_t addr, uint32_t reg, uint8_t *buf, int len)
{
    int i = 0;
    int time_count = 0;
    int time_max = 10000;
    int res;

    /* wait until I2C bus is idle */
    while(i2c_flag_get(ptr->i2c_periph, I2C_FLAG_I2CBSY)) {
        if(time_count > time_max) {
            res = 1;
            goto cmd_fail;

            //              return 1;
        } else {
            time_count++;
        }
    }
    /* send a start condition to I2C bus */
    i2c_start_on_bus(ptr->i2c_periph);
    /* wait until SBSEND bit is set */
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_SBSEND)) {
        if(time_count > time_max) {
            res = 2;
            goto cmd_fail;

            //              return 2;
        } else {
            time_count++;
        }
    }
    /* send slave address to I2C bus */
    i2c_master_addressing(ptr->i2c_periph, addr << 1, I2C_TRANSMITTER);
    /* wait until ADDSEND bit is set */
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_ADDSEND)) {
        if(time_count > time_max) {
            res = 3;
            goto cmd_fail;

            //              return 3;
        } else {
            time_count++;
        }
    }
    /* clear ADDSEND bit */
    i2c_flag_clear(ptr->i2c_periph, I2C_FLAG_ADDSEND);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_TBE)) {
        if(time_count > time_max) {
            res = 4;
            goto cmd_fail;

            //              return 4;
        } else {
            time_count++;
        }
    }
    /* reg transmission */
    i2c_data_transmit(ptr->i2c_periph, reg);
    /* wait until the transmit data buffer is empty */
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_TBE)) {
        if(time_count > time_max) {
            res = 5;
            goto cmd_fail;

            //              return 5;
        } else {
            time_count++;
        }
    }
    for(i = 0; i < len; i++) {
        /* data transmission */
        i2c_data_transmit(ptr->i2c_periph, buf[i]);
        /* wait until the TBE bit is set */
        while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_TBE)) {
            if(time_count > time_max) {
                res = 7;
                goto cmd_fail;

                //              return 7;
            } else {
                time_count++;
            }
        }
    }
    /* send a stop condition to I2C bus */
    i2c_stop_on_bus(ptr->i2c_periph);
    while(I2C_CTL0(ptr->i2c_periph) & I2C_CTL0_STOP) {
        if(time_count > time_max) {
            res = 8;
            goto cmd_fail;

            //              return 8;
        } else {
            time_count++;
        }
    }


    return 0;
cmd_fail:
    i2c_stop_on_bus(ptr->i2c_periph);
    return res;
}

uint32_t bsp_i2c_hw_read_buf(i2c_hw_class_t *ptr, uint8_t addr, uint32_t reg, uint8_t *buf, int len)
{
    int i = len;
    int time_count = 0;
    int time_max = 10000;
    while(i2c_flag_get(ptr->i2c_periph, I2C_FLAG_I2CBSY)) {
        if(time_count > time_max) {
            return 1;
        } else {
            time_count++;
        }
    }
    i2c_start_on_bus(ptr->i2c_periph);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_SBSEND)) {
        if(time_count > time_max) {
            return 2;
        } else {
            time_count++;
        }
    }
    i2c_master_addressing(ptr->i2c_periph, addr << 1, I2C_TRANSMITTER);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_ADDSEND)) {
        if(time_count > time_max) {
            return 3;
        } else {
            time_count++;
        }
    }
    i2c_flag_clear(ptr->i2c_periph, I2C_FLAG_ADDSEND);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_TBE)) {
        if(time_count > time_max) {
            return 4;
        } else {
            time_count++;
        }
    }
    i2c_data_transmit(ptr->i2c_periph, reg);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_BTC)) {
        if(time_count > time_max) {
            return 5;
        } else {
            time_count++;
        }
    }
    i2c_start_on_bus(ptr->i2c_periph);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_SBSEND)) {
        if(time_count > time_max) {
            return 6;
        } else {
            time_count++;
        }
    }
    /* send slave address to I2C bus */
    i2c_master_addressing(ptr->i2c_periph, addr << 1, I2C_RECEIVER);
    while(!i2c_flag_get(ptr->i2c_periph, I2C_FLAG_ADDSEND)) {
        if(time_count > time_max) {
            return 7;
        } else {
            time_count++;
        }
    }
    i2c_flag_clear(ptr->i2c_periph, I2C_FLAG_ADDSEND);
    i2c_ackpos_config(ptr->i2c_periph, I2C_ACKPOS_NEXT);
    while(i) {
        if(1 == i) {
            i2c_ackpos_config(ptr->i2c_periph, I2C_ACKPOS_CURRENT);
            i2c_ack_config(ptr->i2c_periph, I2C_ACK_DISABLE);
        }
        if(i2c_flag_get(ptr->i2c_periph, I2C_FLAG_RBNE)) {
            *buf = i2c_data_receive(ptr->i2c_periph);
            i2c_ack_config(ptr->i2c_periph, I2C_ACK_ENABLE);
            buf++;
            i--;
        }
    }
    i2c_stop_on_bus(ptr->i2c_periph);
    while(I2C_CTL0(ptr->i2c_periph) & 0x0200);
    return 0;
}



#endif
