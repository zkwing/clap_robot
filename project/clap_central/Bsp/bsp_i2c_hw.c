/********************************************************************************************
* include:
********************************************************************************************/
#include "bsp_i2c_hw.h"
#include "drv_log.h"

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
void bsp_i2c_hw_init(i2c_hw_class_t *ptr, I2C_HandleTypeDef *pI2c,uint32_t addr)
{
    ptr->pI2C = pI2c;
    ptr->address = addr;
}

int8_t bsp_i2c_hw_write_buf(i2c_hw_class_t *ptr, uint8_t reg, uint8_t *buf, int len)
{
  uint8_t addr = ptr->address<<1;
//    if(reg == NULL){
//        if(HAL_I2C_Master_Transmit(ptr->pI2C,ptr->address,buf,len,1000)!=HAL_OK){
//            return -1;
//        }
//    }else{
        if(HAL_I2C_Mem_Write(ptr->pI2C,addr,reg,I2C_MEMADD_SIZE_8BIT,buf,len,0xffff)!=HAL_OK){
            return -1;
        }
//    }

    return 0;
}

int8_t bsp_i2c_hw_read_buf(i2c_hw_class_t *ptr, uint8_t reg, uint8_t *buf, int len)
{
  uint8_t addr = (ptr->address<<1) | 0x01;
//    if(reg==0xff){
//        if(HAL_I2C_Master_Receive(ptr->pI2C,ptr->address,buf,len,1000)!=HAL_OK){
//            return -1;
//        }
//    }else{
  HAL_I2C_Mem_Read(ptr->pI2C,addr,reg,I2C_MEMADD_SIZE_8BIT,buf,len,0xffff);
//        if(HAL_I2C_Mem_Read(ptr->pI2C,ptr->address,reg,I2C_MEMADD_SIZE_8BIT,buf,len,0xffff)!=HAL_OK){
//            return -1;
//        }
//    }
    return 0;
}

