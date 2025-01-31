#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>


#define BIT0 (1U << 0) // 0000 0001
#define BIT1 (1U << 1) // 0000 0010
#define BIT2 (1U << 2) // 0000 0100
#define BIT3 (1U << 3) // 0000 1000
#define BIT4 (1U << 4) // 0001 0000
#define BIT5 (1U << 5) // 0010 0000
#define BIT6 (1U << 6) // 0100 0000
#define BIT7 (1U << 7) // 1000 0000

// DS1307 I2C Address
#define DS1307_I2C_ADDRESS      0x68  // 7-bit address (0xD0 >> 1)

// Timekeeper Register Addresses
#define DS1307_REG_SECONDS      0x00
#define DS1307_REG_MINUTES      0x01
#define DS1307_REG_HOURS        0x02
#define DS1307_REG_DAY          0x03
#define DS1307_REG_DATE         0x04
#define DS1307_REG_MONTH        0x05
#define DS1307_REG_YEAR         0x06
#define DS1307_REG_CONTROL      0x07

// 0x00
#define DS1307_CH         (BIT7)
#define DS1307_TEN_SEC    (BIT6 | BIT5 | BIT4)
#define DS1307_SEC        (BIT3 | BIT2 | BIT1 | BIT0)

// 0x01
#define DS1307_TEN_MIN    (BIT6 | BIT5 | BIT4)
#define DS1307_MIN        (BIT3 | BIT2 | BIT1 | BIT0)
// 0x02
#define DS1307_MODE12_24  (BIT6)
#define DS1307_TEN_HR     (BIT5 | BIT4)
#define DS1307_HR         (BIT3 | BIT2 | BIT1 | BIT0)
#define DS1307_AM_PM      (BIT5)
// 0x03
#define DS1307_TEN_DATE   (BIT5 | BIT4)
#define DS1307_DATE       (BIT3 | BIT2 | BIT1 | BIT0)
/// 0x04
#define DS1307_TEN_MONTH  (BIT5)
#define DS1307_MONTH      (BIT3 | BIT2 | BIT1 | BIT0)
// 0x05
#define DS1307_TEN_YR     (BIT7 | BIT6)
#define DS1307_YR         (BIT5 | BIT4 | BIT3 | BIT2 | BIT1 | BIT0)
// 0x06
#define DS1307_OUT        (BIT7)
#define DS1307_SQWE       (BIT4)
#define DS1307_RS1        (BIT1)
#define DS1307_RS0        (BIT0)

// Square Wave Output Rate
#define SQW_RATE_1HZ      (DS1307_SQWE | 0)
#define SQW_RATE_4KHZ     (DS1307_SQWE | DS1307_RS0)
#define SQW_RATE_8KHZ     (DS1307_SQWE | DS1307_RS1)
#define SQW_RATE_32KHZ    (DS1307_SQWE | DS1307_RS1 | DS1307_RS0)

// Utility Macros
#define BCD_TO_BINARY(val)      (((val) >> 4) * 10 + ((val) & 0x0F))
#define BINARY_TO_BCD(val)      ((((val) / 10) << 4) | ((val) % 10))

#endif // DS1307_H
