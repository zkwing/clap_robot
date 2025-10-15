#include "drv_adxl345.h"

static void drv_adxl345_write(drv_adxl345_t *pdr,uint16_t reg,uint8_t *buf,uint8_t size)
{
    bsp_i2c_hw_write_buf(pdr->i2c_interface,reg,buf,size);
}

static void drv_adxl345_read(drv_adxl345_t *pdr,uint16_t reg,uint8_t *buf,uint16_t size)
{
    bsp_i2c_hw_read_buf(pdr->i2c_interface,reg,buf,size);
}

int8_t drv_adxl345_init(drv_adxl345_t *pdr,i2c_hw_class_t *pinf)
{
    if(pdr == NULL || pinf == NULL){
        log_e("drv_adxl345_init param error");
        return -1;
    }
    pdr->i2c_interface = pinf;
    uint8_t reg_value = 0;
    reg_value = 0x0C; // 带宽 200hz 输出速率 400hz
    drv_adxl345_write(pdr,0x2c,&reg_value,1);
    reg_value = 0x08; // 
    drv_adxl345_write(pdr,0x2d,&reg_value,1);
    reg_value = 0x80; // 不使用中断
    drv_adxl345_write(pdr,0x2e,&reg_value,1);
    reg_value = 0x0A; // 全分辨率，8g量程
    drv_adxl345_write(pdr,0x31,&reg_value,1);
    return 0;
}

int8_t drv_adxl345_update(drv_adxl345_t *pdr)
{
    if(pdr == NULL){
        log_e("drv_adxl345_update param error");
        return -1;
    }
    uint8_t buf[6] = {0};
    int16_t temp_data[3] = {0};
//    pdr->cali_flag = 1;
    if(pdr->cali_flag){
        drv_adxl345_read(pdr,0x32,buf,6);
        temp_data[0] = (int16_t)(buf[1]<<8 | buf[0]);
        temp_data[1] = (int16_t)(buf[3]<<8 | buf[2]);
        temp_data[2] = (int16_t)(buf[5]<<8 | buf[4]);
//        log_i("acc raw data:%d,%d,%d\n",temp_data[0],temp_data[1],temp_data[2]);
        pdr->raw_acc_data[0] = temp_data[0] - pdr->raw_acc_offset_data[0];
        pdr->raw_acc_data[1] = temp_data[1] - pdr->raw_acc_offset_data[1];
        pdr->raw_acc_data[2] = temp_data[2] - pdr->raw_acc_offset_data[2];
        pdr->scale_acc_data[0] = pdr->raw_acc_data[0] * 1.0f/256;
        pdr->scale_acc_data[1] = pdr->raw_acc_data[1] * 1.0f/256;
        pdr->scale_acc_data[2] = pdr->raw_acc_data[2] * 1.0f/256;
    }else{
        drv_adxl345_cali(pdr);
        
    }
    return 0;
}

int8_t drv_adxl345_cali(drv_adxl345_t *pdr)
{
    if(pdr == NULL){
        log_e("drv_adxl345_cali param error");
        return -1;
    }
    static uint8_t cnt = 0;
    static int16_t sum_array[3];
    uint8_t buf[6] = {0};
    cnt++;
    if(cnt < 20){  
        drv_adxl345_read(pdr,0x32,buf,6);
    }else if(cnt < 120){
        drv_adxl345_read(pdr,0x32,buf,6);
        sum_array[0] += (int16_t)(buf[1]<<8 | buf[0]);
        sum_array[1] += (int16_t)(buf[3]<<8 | buf[2]);
        sum_array[2] += (int16_t)(buf[5]<<8 | buf[4]);
    }else{
        pdr->raw_acc_offset_data[0] = sum_array[0]/100;
        pdr->raw_acc_offset_data[1] = sum_array[1]/100;
        pdr->raw_acc_offset_data[2] = sum_array[2]/100;
        cnt = 0;
        pdr->cali_flag = 1;
        log_i("drv_adxl345_cali success");
    }

    return 0;
}
