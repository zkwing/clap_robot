#include "drv_adxl345.h"

static void drv_adxl345_write(drv_adxl345_t *pdr,uint16_t reg,uint8_t *buf,uint16_t size)
{
  
}

static void drv_adxl345_read(drv_adxl345_t *pdr,uint16_t reg,uint8_t *buf,uint16_t size)
{

}

int8_t drv_adxl345_init(drv_adxl345_t *pdr,i2c_hw_class_t *pinf)
{
    pdr->i2c_interface = pinf;

}

int8_t drv_adxl345_update(drv_adxl345_t *pdr)
{

} 