/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/12/25
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#include "debug.h"
#include "i2c_driver.h"
#include "ds1307.h"




typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t control;
} DateTime;

/*********************************************************************
 * @fn      I2C_Initialization
 *
 * @brief   Initializes I2C1 peripheral with specified settings.
 *          Configures GPIO pins for SCL (PC2) and SDA (PC1) in 
 *          alternate function open-drain mode.
 *          Configures I2C1 peripheral with 100kHz clock speed.
 * @return  None
 *********************************************************************/
void I2C_Initialization(void) {
    GPIO_InitTypeDef GPIO_InitStruct;
    I2C_InitTypeDef I2C_InitStruct;

    // Enable GPIO & I2C clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // Constract GPIO init structure
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Construct I2C1 init structure
    I2C_InitStruct.I2C_ClockSpeed = 100000;
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 = 0x10;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    

    I2C_Init(I2C1, &I2C_InitStruct);

    // Ebable I2C1
    I2C_Cmd(I2C1, ENABLE);
}

void USARTx_CFG(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);
    GPIO_PinRemapConfig(GPIO_PartialRemap2_USART1, ENABLE);
    /* USART1 TX-->D.6 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}



// Use pointers to directly update the TimekeepeRegisters
void DS1307_ReadRegister(uint8_t* buff) {
    I2C_Start();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Transmitter);
    I2C_SendByte(DS1307_REG_SECONDS);
    I2C_Restart();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Receiver);
    I2C_ReadMultiBytes(buff, 8);
    I2C_Stop();
}

uint8_t data;
void ReadRegister(uint8_t* regAddr) {
    I2C_Start();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Transmitter);
    I2C_SendMultiBytes(regAddr, 1);
    I2C_Restart();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Receiver);
    I2C_ReadMultiBytes(&data, 1);
    I2C_Stop();
}

void UpdateDateTime(DateTime* dt, uint8_t* ds1307Ptr) {
    dt->seconds =   BCD_TO_BINARY(ds1307Ptr[DS1307_REG_SECONDS] & 0x7F);
    dt->minutes =   BCD_TO_BINARY(ds1307Ptr[DS1307_REG_MINUTES] & 0x7F);
    dt->hours =     BCD_TO_BINARY(ds1307Ptr[DS1307_REG_HOURS]);
    dt->day =       BCD_TO_BINARY(ds1307Ptr[DS1307_REG_DAY]);
    dt->date =      BCD_TO_BINARY(ds1307Ptr[DS1307_REG_DATE]);
    dt->month =     BCD_TO_BINARY(ds1307Ptr[DS1307_REG_MONTH]);
    dt->year =      BCD_TO_BINARY(ds1307Ptr[DS1307_REG_YEAR]);
    dt->control =   ds1307Ptr[DS1307_REG_CONTROL];
}

void DS1307_SetTime(DateTime* dt) {
    I2C_Start();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Transmitter);
    I2C_SendMultiBytes((uint8_t*)dt, sizeof(DateTime));
    I2C_Stop();
}



void DS1307_Init() {
    I2C_Start();
    printf("Start");
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Transmitter);
    printf("Set Transmitter");
    I2C_SendByte(DS1307_REG_SECONDS);
    printf("Send Address");
    I2C_SendByte(0x00);
    printf("send byte");
    I2C_Stop();

    // Set SQW Rate to 1Hz
    I2C_Start();
    I2C_SendSlaveAddress(DS1307_I2C_ADDRESS << 1, I2C_Direction_Transmitter);
    I2C_SendByte(DS1307_REG_CONTROL);
    I2C_SendByte(SQW_RATE_1HZ);
    I2C_Stop();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */


//#define DEBUG DEBUG_UART1_Remap2

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();
    I2C_Initialization();

// #if (SDI_PRINT == SDI_PR_OPEN)
//     SDI_Printf_Enable();
// #else
    USART_Printf_Init(115200);
// #endif
    printf("SystemClk:%d\r\n", SystemCoreClock);    
    printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
    
    // Generate buffer
    DateTime dateTime = {0};
    uint8_t dt_buffer[8] = {0};
    DS1307_Init();
    printf("Seccess Init");
    while(1)
    {   
        DS1307_ReadRegister(dt_buffer);
        UpdateDateTime(&dateTime, dt_buffer);
        Delay_Ms(1000);
        printf("Time: %02d:%02d:%02d\r\n", dateTime.hours, dateTime.minutes, dateTime.seconds);
    }
}
