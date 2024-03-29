#ifndef XI2C_H_
#define XI2C_H_

#include "driver/i2c.h"
#include "stdio.h"
#include "string.h"
#include "esp_system.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */

#define ACK_VAL    I2C_MASTER_ACK         /*!< I2C ack value */
#define NACK_VAL   I2C_MASTER_NACK         /*!< I2C nack value */




esp_err_t X_WriteMulti(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint32_t count, uint8_t* data_wr);
esp_err_t X_ReadMulti(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint32_t count, uint8_t* data_rd);
esp_err_t X_WrByte(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t data_wr);
esp_err_t X_RdByte(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t *data_rd);
esp_err_t X_WrBit(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t bitNum, uint8_t data_wr);
esp_err_t X_WrBits(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t bitStart, uint8_t length, uint8_t data_wr);
esp_err_t X_WrWord(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t data_wr);
esp_err_t X_WrDWord(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t data_wr);
esp_err_t X_RdBit(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t bitNum, uint8_t *data_rd);
esp_err_t X_RdBits(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint8_t bitStart, uint8_t length, uint8_t *data_rd);
esp_err_t X_RdWord(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint16_t *data_rd);
esp_err_t X_RdDWord(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t index, uint32_t *data_rd);
esp_err_t X_PollingDelay(void);

esp_err_t XI2CWrite(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t* data_wr, size_t size);
 esp_err_t XI2CRead(i2c_port_t i2c_num, uint8_t i2c_add, uint8_t* data_rd, size_t size);

#endif /* I2C_H_ */
