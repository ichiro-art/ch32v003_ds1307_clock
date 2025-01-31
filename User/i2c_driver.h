#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

#include <stdint.h>

void    I2C_Start(void);
void    I2C_Restart(void);
void    I2C_SendSlaveAddress(uint8_t slaveAddr, uint8_t direction);
void    I2C_SendByte(uint8_t byte);
void    I2C_SendMultiBytes(const uint8_t* buf, size_t len);
void    I2C_ReadByte(uint8_t byte);
void    I2C_ReadMultiBytes(uint8_t* buf, size_t len);
void    I2C_Stop(void);

#endif // I2C_DRIVER_H
