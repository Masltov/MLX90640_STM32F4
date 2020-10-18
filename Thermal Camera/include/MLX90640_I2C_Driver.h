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
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _MLX90640_I2C_Driver_H_
#define _MLX90640_I2C_Driver_H_

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

void 	MLX90640_I2CInit(void);
int 	MLX90640_I2CRead(uint8_t slaveAddr,uint16_t startAddress, uint16_t nMemAddressRead, uint16_t *data);
int 	MLX90640_I2CWrite(uint8_t slaveAddr,uint16_t writeAddress, uint16_t data);
void 	MLX90640_I2CFreqSet(int freq);

#endif

#ifdef __cplusplus
}
#endif
