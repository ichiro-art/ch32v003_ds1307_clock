#include "ch32v00x_i2c.h"

/*********************************************************************
 * @fn      I2C_Start
 *
 * @brief   Generates I2C START condition.
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void I2C_Start(void) {
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

/*********************************************************************
 * @fn      I2C_Restart
 *
 * @brief   Generates I2C RESTART condition.
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void I2C_Restart(void) {
    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
}

/*********************************************************************
 * @fn      I2C_SendSlaveAddress
 *
 * @brief   Sends slave address with specified direction.
 *
 * @param   slaveAddr - 7-bit slave address
 * @param   direction - I2C_Direction_Transmitter or I2C_Direction_Receiver
 *
 * @return  None
 *********************************************************************/
void I2C_SendSlaveAddress(uint8_t slaveAddr, uint8_t direction) {
    I2C_Send7bitAddress(I2C1, slaveAddr, direction);
    if(direction == I2C_Direction_Transmitter) {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    } else {
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    }
}

/*********************************************************************
 * @fn      I2C_SendBytes
 *
 * @brief   Sends multiple bytes over I2C.
 *
 * @param   buf - pointer to the data buffer
 * @param   len - number of bytes to send
 *
 * @return  None
 *********************************************************************/
void I2C_SendMultiBytes(uint8_t* buf, size_t len) {
    for(size_t i = 0; i < len; i++) {
        I2C_SendData(I2C1, buf[i]);
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    }
}

/*********************************************************************
 * @fn      I2C_SendByte
 *
 * @brief   Sends a single byte over I2C.
 *
 * @param   byte - byte to send
 *
 * @return  None
 *********************************************************************/
void I2C_SendByte(uint8_t byte) {
    I2C_SendData(I2C1, byte);
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}

/*********************************************************************
 * @fn      I2C_ReadBytes
 *
 * @brief   Reads multiple bytes over I2C.
 *
 * @param   buf - pointer to the data buffer
 * @param   len - number of bytes to read
 *
 * @return  None
 *********************************************************************/
void I2C_ReadMultiBytes(uint8_t* buf, size_t len) {
    for(size_t i = 0; i < len; i++) {
        // If it's the last byte, send NACK; otherwise, send ACK
        if(i == (len - 1)) {
            I2C_AcknowledgeConfig(I2C1, DISABLE);
        } else {
            I2C_AcknowledgeConfig(I2C1, ENABLE);
        }

        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));
        buf[i] = I2C_ReceiveData(I2C1);
    }
}

/*********************************************************************
 * @fn      I2C_Stop
 *
 * @brief   Generates I2C STOP condition.
 *
 * @param   None
 *
 * @return  None
 *********************************************************************/
void I2C_Stop(void) {
    I2C_GenerateSTOP(I2C1, ENABLE);
}