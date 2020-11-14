/*
 * MLX90640_I2C.h
 *
 *  Created on: 29.01.2020
 *      Author: masltov
 */

#ifndef INCLUDE_MLX90640_I2C_H_
#define INCLUDE_MLX90640_I2C_H_

#include "stm32f4xx_hal.h"

#ifndef USED_I2C
#define USED_I2C I2C2
#endif

#ifndef MLX_I2C_CLOCKSPEED
#define MLX_I2C_CLOCKSPEED 400000U
#endif

#ifndef MLX_I2C_TIMEOUT
#define MLX_I2C_TIMEOUT 5000U
#endif

#ifndef MLX_SLAVE_ADDRESS
#define MLX_SLAVE_ADDRESS 0x0033<<1
#endif

#ifndef MLX_OWN_ADDRESS1
#define MLX_OWN_ADDRESS1 1
#endif

#ifndef MLX_OWN_ADDRESS2
#define MLX_OWN_ADDRESS2 2
#endif

#ifndef MLX_ADDRESSING_MODE
#define MLX_ADDRESSING_MODE I2C_ADDRESSINGMODE_7BIT
#endif

#ifndef MLX_DUALADDRESS_MODE
#define MLX_DUALADDRESS_MODE I2C_DUALADDRESS_DISABLE
#endif

#ifndef MLX_DUTYCYCLE
#define MLX_DUTYCYCLE I2C_DUTYCYCLE_2
#endif
#ifndef MLX_CALL_MODE
#define MLX_CALL_MODE I2C_GENERALCALL_DISABLE
#endif

#ifndef MLX_NOSTRETCH_MODE
#define MLX_NOSTRETCH_MODE I2C_NOSTRETCH_DISABLE
#endif

#ifndef MLX_MEMADD_SIZE
#define MLX_MEMADD_SIZE 0x0010
#endif

#ifndef MLX_WORD_SIZE
#define MLX_WORD_SIZE 0x0010
#endif

enum{FALSE, TRUE};


typedef union rxDataMLX
{
	uint8_t 	rx8Data[0x37F*2];
	uint16_t	rx16Data[0x37F];
} MLX_RXDATA;

typedef struct mlx_i2cInterface
{
	uint8_t 			isInitialized;
	uint8_t 			hasValidData;
	HAL_StatusTypeDef 	State;
	I2C_HandleTypeDef 	hi2c;
	uint8_t* 			rxData;
	uint16_t*			rx16Data;
	uint8_t* 			txData;
	float 				tempMap[24][32];
}MLX_I2C;

void MLX90640_I2CInit(void);
MLX_I2C* mlx_init(uint8_t *rx_Data, uint16_t *rx16_Data, uint8_t *tx_Data);
void updateTemp(void);
void mlx_getImage();
void mlx_calibrate(void);
void mlx_setReg(uint16_t RegAddr);
uint16_t mlx_readReg(uint16_t RegAddr);

#endif /* INCLUDE_MLX90640_I2C_H_ */
