/*
 * Communication.h
 *
 *  Created on: 19.01.2020
 *      Author: masltov
 */

#ifndef INCLUDE_COMMUNICATION_H_
#define INCLUDE_COMMUNICATION_H_
#include "main.h"

struct comInterface
{
	int isInitializedIR;
	int isInitializedLCD;
};



/* Common Error codes */
#define ERROR_NONE                    0
#define ERROR_NO_INIT                -1
#define ERROR_WRONG_PARAM            -2
#define ERROR_BUSY                   -3
#define ERROR_PERIPH_FAILURE         -4
#define ERROR_COMPONENT_FAILURE      -5
#define ERROR_UNKNOWN_FAILURE        -6
#define ERROR_UNKNOWN_COMPONENT      -7
#define ERROR_BUS_FAILURE            -8
#define ERROR_CLOCK_FAILURE          -9
#define ERROR_MSP_FAILURE            -10
#define ERROR_FEATURE_NOT_SUPPORTED  -11


/* BSP BUS error codes */

#define ERROR_BUS_TRANSACTION_FAILURE    -100
#define ERROR_BUS_ARBITRATION_LOSS       -101
#define ERROR_BUS_ACKNOWLEDGE_FAILURE    -102
#define ERROR_BUS_PROTOCOL_FAILURE       -103

#define ERROR_BUS_MODE_FAULT             -104
#define ERROR_BUS_FRAME_ERROR            -105
#define ERROR_BUS_CRC_ERROR              -106
#define ERROR_BUS_DMA_FAILURE            -107


#ifndef BUS_SPI2_BAUDRATE
#define BUS_SPI2_BAUDRATE                 10000000U /* baud rate of SPI2 = 10 Mbps*/
#endif


#define BUS_I2C2                          I2C2
#define BUS_I2C2_CLOCK_ENABLE()           __HAL_RCC_I2C2_CLK_ENABLE()
#define BUS_I2C2_CLOCK_DISABLE()          __HAL_RCC_I2C2_CLK_DISABLE()
#define BUS_I2C2_FORCE_RESET()            __HAL_RCC_I2C2_FORCE_RESET()
#define BUS_I2C2_RELEASE_RESET()          __HAL_RCC_I2C2_RELEASE_RESET()
#define BUS_I2C2_SDA_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C2_SCL_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_I2C2_SDA_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_I2C2_SCL_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()

#define BUS_I2C2_SCL_GPIO_PIN             GPIO_PIN_10
#define BUS_I2C2_SCL_GPIO_PORT            GPIOB
#define BUS_I2C2_SCL_GPIO_AF              GPIO_AF4_I2C2
#define BUS_I2C2_SDA_GPIO_PIN             GPIO_PIN_11
#define BUS_I2C2_SDA_GPIO_PORT            GPIOB
#define BUS_I2C2_SDA_GPIO_AF              GPIO_AF4_I2C2

#ifndef BUS_I2C2_FREQUENCY
#define BUS_I2C2_FREQUENCY                400000U /* Frequency of I2C2 = 400 KHz*/
#endif /* BUS_I2C2_FREQUENCY */

#ifndef BUS_I2C2_POLL_TIMEOUT
#define BUS_I2C2_POLL_TIMEOUT             0x1000U
#endif /* BUS_I2C2_POLL_TIMEOUT */

#define BUS_SPI2                          SPI2
#define BUS_SPI2_CLK_ENABLE()             __HAL_RCC_SPI2_CLK_ENABLE()
#define BUS_SPI2_CLK_DISABLE()            __HAL_RCC_SPI2_CLK_DISABLE()
#define BUS_SPI2_SCK_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_NSS_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()
#define BUS_SPI2_SCK_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_SPI2_NSS_GPIO_CLK_DISABLE()   __HAL_RCC_GPIOB_CLK_DISABLE()
#define BUS_SPI2_MISO_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI2_MOSI_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()
#define BUS_SPI2_MISO_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI2_MOSI_GPIO_CLK_DISABLE()  __HAL_RCC_GPIOC_CLK_DISABLE()
#define BUS_SPI2_SCK_GPIO_PIN             GPIO_PIN_13
#define BUS_SPI2_NSS_GPIO_PIN             GPIO_PIN_12
#define BUS_SPI2_MISO_GPIO_PIN            GPIO_PIN_2
#define BUS_SPI2_MOSI_GPIO_PIN            GPIO_PIN_3
#define BUS_SPI2_MISO_GPIO_PORT           GPIOC
#define BUS_SPI2_MOSI_GPIO_PORT           GPIOC
#define BUS_SPI2_SCK_GPIO_PORT            GPIOB
#define BUS_SPI2_NSS_GPIO_PORT            GPIOB
#define BUS_SPI2_SCK_GPIO_AF              GPIO_AF5_SPI2
#define BUS_SPI2_MISO_GPIO_AF             GPIO_AF5_SPI2
#define BUS_SPI2_MOSI_GPIO_AF             GPIO_AF5_SPI2

#ifndef SPI2_POLL_TIMEOUT
   #define SPI2_POLL_TIMEOUT          ((uint32_t)0x1000)
#endif /* SPI2_POLL_TIMEOUT */

/* --------- Function Prototypes for SPI2 -------- */
int32_t SPI2_Init(void);
void SPI2_SetDataWidth(uint32_t w16);
int32_t SPI2_Send(uint8_t *pData, uint16_t Length);



/* --------- Function Prototypes for I2C_2 -------- */
HAL_StatusTypeDef MX_I2C2_Init(I2C_HandleTypeDef* hi2c);
int8_t IR_I2C2_Init(void);
int8_t IR_I2C2_DeInit(void);
int8_t I2C_IT_init(void);
int8_t IR_I2C2_IsReady(uint16_t DevAddr, uint32_t Trials);
int8_t IR_I2C2_WriteReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int8_t IR_I2C2_ReadReg(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int8_t IR_I2C2_WriteReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int8_t IR_I2C2_ReadReg16(uint16_t DevAddr, uint16_t Reg, uint8_t *pData, uint16_t Length);
int8_t IR_I2C2_Send(uint16_t DevAddr, uint8_t *pData, uint16_t Length);
int8_t IR_I2C2_Recv(uint16_t DevAddr, uint8_t *pData, uint16_t Length);

/* Delacration of external Variables */


#endif /* INCLUDE_COMMUNICATION_H_ */
