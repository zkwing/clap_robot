#include "drv_itg3205.h"

#define GYRO_SCALE 1/14.375f

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
    reg_value = 0x80;
    drv_itg3205_write(pdr,0x3E,&reg_value,1);
    reg_value = 0x07;
    drv_itg3205_write(pdr,0x15,&reg_value,1);
    reg_value = 0x1E;
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
        log_e("drv_itg3205_update input param null");
        return -1;
    }
    uint8_t buf[6];
    if(pdr->cali_flag){
      int16_t temp_data[3];
      drv_itg3205_read(pdr,0x1D,buf,6);
      temp_data[0] = (int16_t)(buf[0] << 8 | buf[1]);
      temp_data[1] = (int16_t)(buf[2] << 8 | buf[3]);
      temp_data[2] = (int16_t)(buf[4] << 8 | buf[5]);
      pdr->raw_gyro_data[0] = temp_data[0] - pdr->raw_gyro_offset_data[0];
      pdr->raw_gyro_data[1] = temp_data[1] - pdr->raw_gyro_offset_data[1];
      pdr->raw_gyro_data[2] = temp_data[2] - pdr->raw_gyro_offset_data[2];
      pdr->scale_gyro_data[0] = pdr->raw_gyro_data[0] * GYRO_SCALE;
      pdr->scale_gyro_data[1] = pdr->raw_gyro_data[1] * GYRO_SCALE;
      pdr->scale_gyro_data[2] = pdr->raw_gyro_data[2] * GYRO_SCALE;
//      jcom_float_print(1,pdr->scale_gyro_data[0],pdr->scale_gyro_data[1],pdr->scale_gyro_data[2]);
    }else{
      drv_itg3205_cali(pdr);
    }
    return 0;
}

int8_t drv_itg3205_cali(drv_itg3205_t *pdr)
{
    static uint16_t cnt=0;
    if(pdr == NULL){
        log_e("drv_itg3205_cali input param null");
        return -1;
    }
    
    uint8_t buf[6];
    static int16_t sum_array[3];
    cnt++;
    if(cnt<20){
      drv_itg3205_read(pdr,0x1D,buf,6);
    }else if(cnt < 120){
        drv_itg3205_read(pdr,0x1D,buf,6);
        sum_array[0] += (int16_t)(buf[0] << 8 | buf[1]);
        sum_array[1] += (int16_t)(buf[2] << 8 | buf[3]);
        sum_array[2] += (int16_t)(buf[4] << 8 | buf[5]);

    }else{
        pdr->raw_gyro_offset_data[0] = sum_array[0]/100;
        pdr->raw_gyro_offset_data[1] = sum_array[1]/100;
        pdr->raw_gyro_offset_data[2] = sum_array[2]/100;
        pdr->cali_flag = 1;
        log_i("drv_itg3205_cali success");
    }

    
    return 0;
}
