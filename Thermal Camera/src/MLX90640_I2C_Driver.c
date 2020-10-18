/**
 * @copyright (C) 2017 Melexis N.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "MLX90640_I2C_Driver.h"
//#include "MLX90640_I2C.h"

I2C_HandleTypeDef hi2c;

void MLX90640_I2CInit(void)
{   
	//Initialize Hardware
		GPIO_InitTypeDef gpio_init;

	    /* Configure GPIO B10 as SCL of I2C2 */
	    __HAL_RCC_GPIOB_CLK_ENABLE();
	    gpio_init.Pin     = GPIO_PIN_10;
	    gpio_init.Mode    = GPIO_MODE_AF_OD;
	    gpio_init.Pull    = GPIO_NOPULL;
	    gpio_init.Speed   = GPIO_SPEED_FREQ_HIGH;
	    gpio_init.Alternate = GPIO_AF4_I2C2;
	    HAL_GPIO_Init(GPIOB, &gpio_init);
	    __HAL_RCC_I2C2_CLK_ENABLE();

	    /* Configure GPIO B11 as SDA of I2C2 */
	    gpio_init.Pin     = GPIO_PIN_11;
	    gpio_init.Mode    = GPIO_MODE_AF_OD;
	    gpio_init.Pull    = GPIO_NOPULL;
	    gpio_init.Speed   = GPIO_SPEED_FREQ_HIGH;
	    gpio_init.Alternate = GPIO_AF4_I2C2;
	    HAL_GPIO_Init(GPIOB, &gpio_init);

		//Initialize i2c InitTypeDef
		I2C_InitTypeDef init_i2c;
		init_i2c.ClockSpeed = MLX_I2C_CLOCKSPEED;
		init_i2c.AddressingMode = MLX_ADDRESSING_MODE;
		init_i2c.DualAddressMode = MLX_DUALADDRESS_MODE;
		init_i2c.OwnAddress1 = MLX_OWN_ADDRESS1;
		init_i2c.OwnAddress2 = MLX_OWN_ADDRESS2;
		init_i2c.DutyCycle = MLX_DUTYCYCLE;
		init_i2c.GeneralCallMode = MLX_CALL_MODE;
		init_i2c.NoStretchMode = MLX_NOSTRETCH_MODE;

		//Initialize i2c HandleTypeDef and Instance
		hi2c.Instance = USED_I2C;
		hi2c.Init = init_i2c;
		HAL_I2C_Init(&hi2c);


}

int MLX90640_I2CRead(uint8_t slaveAddr, uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data)
{

    int i = 0;

    uint8_t i2cData[1664] = {0};
    uint16_t *p;
    uint8_t *p8 = &i2cData[0];
    p = data;

    if(HAL_I2C_Mem_Read(&hi2c, MLX_SLAVE_ADDRESS, startAddress, MLX_MEMADD_SIZE, p8, nMemAddressRead*2, MLX_I2C_TIMEOUT)!=HAL_OK)
    {
    	return -1;
    }

    for(int cnt=0; cnt < nMemAddressRead; cnt++)
    {
        i = cnt << 1;
        *p++ = (uint16_t)i2cData[i]*256 + (uint16_t)i2cData[i+1];
    }
    
    return 0;   
} 

void MLX90640_I2CFreqSet(int freq)
{
    //i2c.frequency(1000*freq);
}

int MLX90640_I2CWrite(uint8_t slaveAddr, uint16_t writeAddress, uint16_t data)
{
    uint8_t cmd[2] = {0};
    static uint16_t dataCheck;


    cmd[0] = data >> 8;
    cmd[1] = data & 0x00FF;

    uint8_t *p = &cmd[0];
    
    if (HAL_I2C_Mem_Write(&hi2c, MLX_SLAVE_ADDRESS,writeAddress, MLX_MEMADD_SIZE,p, 2, MLX_I2C_TIMEOUT) != HAL_OK)
    {
        return -1;
    }         
    
    MLX90640_I2CRead(slaveAddr,writeAddress,1, &dataCheck);
    
    if ( dataCheck != data)
    {
        return -2;
    }    
    
    return 0;
}

