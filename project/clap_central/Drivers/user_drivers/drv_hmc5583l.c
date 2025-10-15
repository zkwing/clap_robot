#include "drv_hmc5583l.h"

static void drv_hmc5583l_write(drv_hmc5583l_t *pdr,uint16_t reg,uint8_t *buf,uint8_t size)
{
    bsp_i2c_hw_write_buf(pdr->i2c_interface,reg,buf,size);
}

static void drv_hmc5583l_read(drv_hmc5583l_t *pdr,uint16_t reg,uint8_t *buf,uint16_t size)
{
    bsp_i2c_hw_read_buf(pdr->i2c_interface,reg,buf,size);
}



int8_t drv_hmc5583l_init(drv_hmc5583l_t *pdr, i2c_hw_class_t *pinf)
{
    if(pdr == NULL || pinf == NULL){
        log_e("drv_hmc5583l_init param error");
        return -1;
    }
    pdr->i2c_interface = pinf;
    uint8_t reg_value = 0;
    reg_value = 0x1C;
    drv_hmc5583l_write(pdr,0x00,&reg_value,1);
    reg_value = 0xE0; // 8 Ga  230Counts/高斯
    drv_hmc5583l_write(pdr,0x01,&reg_value,1);
    reg_value = 0x01; // 单次测量模式
    drv_hmc5583l_write(pdr,0x02,&reg_value,1);
    return 0;

}

int8_t drv_hmc5583l_update(drv_hmc5583l_t *pdr)
{
    uint8_t buf[6];
    uint8_t reg_value = 0;
    int16_t temp_data[3];
    drv_hmc5583l_read(pdr,0x09,&reg_value,1);
    if((reg_value & 0x01) == 0x00){
        log_e("drv_hmc5583l_update data not ready, 0x09 == %x\n",reg_value);
        return -1;
    }
    drv_hmc5583l_read(pdr,0x03,buf,6);
    temp_data[0] = (int16_t)(buf[1]<<8 | buf[0]);
    temp_data[1] = (int16_t)(buf[3]<<8 | buf[2]);
    temp_data[2] = (int16_t)(buf[5]<<8 | buf[4]);
    pdr->raw_mag_data[0] = temp_data[0] - pdr->raw_mag_offset_data[0];
    pdr->raw_mag_data[1] = temp_data[1] - pdr->raw_mag_offset_data[1];
    pdr->raw_mag_data[2] = temp_data[2] - pdr->raw_mag_offset_data[2];
    pdr->scale_mag_data[0] = pdr->raw_mag_data[0]*1.0f/230;
    pdr->scale_mag_data[1] = pdr->raw_mag_data[1]*1.0f/230;
    pdr->scale_mag_data[2] = pdr->raw_mag_data[2]*1.0f/230;
    reg_value = 0x01; // 单次测量模式
    drv_hmc5583l_write(pdr,0x02,&reg_value,1);
    return 0;
}

int8_t drv_hmc5583l_cali(drv_hmc5583l_t *pdr)
{
    if(pdr == NULL){
        log_e("drv_hmc5583l_cali param error");
        return -1;
    }
    uint8_t buf[6];
    uint8_t reg_value = 0;
    drv_hmc5583l_read(pdr,0x09,&reg_value,1);
    if((reg_value & 0x01) == 0x00){
        log_e("drv_hmc5583l_update data not ready, 0x09 == %x\n",reg_value);
        return -1;
    }
    drv_hmc5583l_read(pdr,0x03,buf,6);
    pdr->raw_mag_offset_data[0] = (int16_t)(buf[1]<<8 | buf[0]);
    pdr->raw_mag_offset_data[1] = (int16_t)(buf[3]<<8 | buf[2]);
    pdr->raw_mag_offset_data[2] = (int16_t)(buf[5]<<8 | buf[4]);
    reg_value = 0x01; // 单次测量模式
    drv_hmc5583l_write(pdr,0x02,&reg_value,1);

    return 0;
}