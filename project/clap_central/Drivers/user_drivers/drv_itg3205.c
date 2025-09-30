#include "drv_itg3205.h"

static void drv_itg3205_write(drv_itg3205_t *pdr,uint16_t reg,uint8_t *buf,uint8_t size)
{
    bsp_i2c_hw_write_buf(pdr->i2c_interface,reg,buf,size);
}

static void drv_itg3205_read(drv_itg3205_t *pdr,uint16_t reg,uint8_t *buf,uint16_t size)
{
    bsp_i2c_hw_read_buf(pdr->i2c_interface,reg,buf,size);
}

int8_t drv_itg3205_init(drv_itg3205_t *pdr,i2c_hw_class_t *pinf)
{
    if(pdr == NULL || pinf == NULL)
    {
        log_e("drv_itg3205_init param error");
        return -1;
    }
    pdr->i2c_interface = pinf;
    uint8_t reg_value = 0;
    reg_value = 0x01;
    drv_itg3205_write(pdr,0x16,&reg_value,1);
    reg_value = 0x00;
    drv_itg3205_write(pdr,0x17,&reg_value,1);
    reg_value = 0x00;
    drv_itg3205_write(pdr,0x3E,&reg_value,1);
    return 0;

}

int8_t drv_itg3205_update(drv_itg3205_t *pdr)
{
    if(pdr == NULL){
        log_e("input param null");
        return -1;
    }
    uint8_t buf[6];
    drv_itg3205_read(pdr,0x1D,buf,6);
    pdr->raw_gyro_data[0] = (int16_t)(buf[0] << 8 | buf[1]);
    pdr->raw_gyro_data[1] = (int16_t)(buf[2] << 8 | buf[3]);
    pdr->raw_gyro_data[2] = (int16_t)(buf[4] << 8 | buf[5]);

    return 0;

}