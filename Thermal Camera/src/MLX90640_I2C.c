#include "MLX90640_I2C.h"
#include "MXL90640_IC.h"
#include <math.h>


/*****************************************
 * START OF VARIABLE DECLARATION  		 *
 * ***************************************/

// -------------Definitions--------------//
typedef struct mlx_internal_struct
{
	MLX_PARAM param;
	float pixArray[24][32];
	float* pPixArray;
} MLX_STRUCT;

// -------------Declarations--------------//
MLX_STRUCT mlxStruct; 	//Internal Interface
MLX_I2C extInterface;	//External Interface
int16_t OS_ref_Array[24][32];
float 	OS_Array[24][32];
float 	K_ta_pix[24][32];
float 	Kv_pix[24][32];
double 	alpha_pix[24][32];
double 	S_pix[24][32];



/*****************************************
 * START OF FUNCTION DECLARATIONS  		 *
 * ***************************************/

//Function Prototype
HAL_StatusTypeDef i2c_init(void);
void i2c_write(uint16_t length);
void i2c_read(uint16_t length);
uint16_t i2c_readReg(uint16_t RegAddr);
void i2c_setReg(uint16_t RegAddr, uint16_t content);
HAL_StatusTypeDef i2c_init();

void MLX90640_SetChessMode(void);
void MLX_GET_EEPROM_CONTENT(void); 	//reads out EEPROM from 0x2400 to 0x273F
void MLX_GET_EEPROM_COEFF(void);	// Extracts and calculates Parameter stored in EEPROM
void getOcc_Row_Vec(void);
void getOcc_Col_Vec(void);
void Calc_Offset_Array(void);
void Calc_K_ta_Array(void);
void Calc_Kv_Array(void);
void Calc_alpha_Array(void);

void MLX_GET_RAM_CONTENT(void);
void MLX_SupplyVDD_Calc(void);
void MLX_Resolution_Calc(void);
void MLX_AmbTemp_Calc(void);
void MLX_Gain_Calc(void);
void MLX_Gain_Compensation(void);
void MLX_readSP0(void);
void MLX_readSP1(void);
void MLX_setReadFinished(uint16_t content);
void MLX_IR_Data_Compensation(void);
void MLX_GET_CPPIX_GAIN_COMP(void);
void MLX_CPPIX_DATA_COMP(void);
void MLX_IR_GRAD_COMP(void);
void MLX_Normalize_Sensitvity(void);
void MLX_Calc_TO(void);



//Start of Function Implementation

/* ******************************************* */
/* ************* Begin of I2C stuff ********** */
/* ******************************************* */
HAL_StatusTypeDef i2c_init()
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
	//init_i2c.ClockSpeed = MLX_I2C_CLOCKSPEED;

	//Initialize i2c HandleTypeDef and Instance
	extInterface.hi2c.Instance = USED_I2C;
	extInterface.hi2c.Init = init_i2c;
	extInterface.State=HAL_I2C_Init(&extInterface.hi2c);

	return extInterface.State;
}

void i2c_write(uint16_t length)
{
	extInterface.State = HAL_I2C_Master_Transmit(&extInterface.hi2c, MLX_SLAVE_ADDRESS, extInterface.txData, length, MLX_I2C_TIMEOUT);
}

void i2c_read(uint16_t length)
{
	extInterface.State = HAL_I2C_Master_Receive(&extInterface.hi2c, MLX_SLAVE_ADDRESS, extInterface.rxData, length, MLX_I2C_TIMEOUT);
}

void i2c_setReg(uint16_t RegAddr, uint16_t content)
{
	extInterface.txData[0] = (uint8_t) (content >> 8);
	extInterface.txData[1] = (uint8_t) (content & 0x00FF);
	extInterface.State = HAL_I2C_Mem_Write(&extInterface.hi2c, MLX_SLAVE_ADDRESS,RegAddr, MLX_MEMADD_SIZE,extInterface.txData, 1, MLX_I2C_TIMEOUT);
}

uint16_t i2c_readReg(uint16_t RegAddr)
{
	uint8_t buf[2];
	uint8_t* pBuf = &buf[0];
	uint16_t retVal;
	extInterface.State = HAL_I2C_Mem_Read(&extInterface.hi2c, MLX_SLAVE_ADDRESS, RegAddr, MLX_MEMADD_SIZE, pBuf, 1, MLX_I2C_TIMEOUT);
	retVal = buf[0] << 8 | buf[1];
	return retVal;
}

void i2c_readRegBlock(uint16_t strtAddr, uint16_t length)
{
	uint8_t bufArr[length*2];
	uint8_t *pBuf = &bufArr[0];
	extInterface.State = HAL_I2C_Mem_Read(&extInterface.hi2c, MLX_SLAVE_ADDRESS, strtAddr, MLX_MEMADD_SIZE, pBuf, length*2, MLX_I2C_TIMEOUT);
	memcpy(extInterface.rxData, pBuf, 0x37F*2);
}

/* ******************************************* */
/* ************* End of I2C stuff ************ */
/* ******************************************* */



/* ******************************************* */
/* **** Begin of MLX Initialization stuff **** */
/* ******************************************* */

MLX_I2C* mlx_init(uint8_t *rx_Data, uint16_t *rx16_Data, uint8_t *tx_Data)
{
	if (i2c_init()==HAL_OK)
	{
		extInterface.isInitialized = TRUE;
		extInterface.hasValidData = FALSE;
		extInterface.rxData = rx_Data;
		extInterface.rx16Data = rx16_Data;
		extInterface.txData = tx_Data;
		MLX90640_SetChessMode();
		//MLX_GET_EEPROM_CONTENT();
		MLX_GET_EEPROM_COEFF();
		return &extInterface;
	}
	else return 0;
}


void MLX90640_SetChessMode()
{
    uint16_t controlRegister1;
    uint16_t value;


    controlRegister1 = i2c_readReg(0x800D);

    value = (controlRegister1 | 0x1000);
    i2c_setReg(0x800D, value);
}

void MLX_GET_EEPROM_CONTENT()
{
	//Read in EEPROM content into rxData
	i2c_readRegBlock(MLX_EEPROM_ADR_STA, (MLX_EEPROM_ADR_END-MLX_EEPROM_ADR_STA + 1));
}

void MLX_GET_EEPROM_COEFF()
{
	uint16_t buf, buf2;

	/*
	 * Get Step 0x243F [13-12]
	 */
	//buf = extInterface.rx16Data[MLX_EE_CT-MLX_EEPROM_ADR_STA];
	buf = i2c_readReg(MLX_EE_CT);
	mlxStruct.param.Step= ((buf & 0x3000) >> 12) * 10;

	/*
	 * Get CT3 0x243F [7-4]
	 */
	mlxStruct.param.CT3 = ((buf & 0x00F0) >> 4) * mlxStruct.param.Step;

	/*
	 * Get CT4 0x243F [11-8]
	 */
	mlxStruct.param.CT4 = (((buf & 0x0F00) >> 8) * mlxStruct.param.Step) + mlxStruct.param.CT3;

	/*
	 * Get Ks_scale_to 0x243F [3-0]
	 */
	mlxStruct.param.Ks_scale_to = (buf & 0x000F) + 8;

	/*
	 * Get  Ks_to4_EE 0x243E [15-8]
	 */
	buf  = i2c_readReg(MLX_EE_Ks_To_4_3);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.Ks_to4_EE = (buf2 > 127 ? (int8_t) (buf2-256) : (int8_t) buf2);

	/*
	 * Get  Ks_to3_EE 0x243E [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.Ks_to3_EE = (buf2 > 127 ? (int8_t) (buf2-256) : (int8_t) buf2);

	/*
	 * Get  Ks_to2_EE 0x243D [15-8]
	 */
	buf  = i2c_readReg(MLX_EE_Ks_To_2_1);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.Ks_to2_EE = buf2 > 127 ? (int8_t) (buf2-256) : (int8_t) buf2;

	/*
	 * Get  Ks_to1_EE 0x243D [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.Ks_to1_EE = buf2 > 127 ? (int8_t) (buf2-256) : (int8_t) buf2;

	/*
	 * Get Ks_ta_EE 0x243C [15-8]
	 */
	buf =  i2c_readReg(MLX_EE_KsTa_TGC);
	buf2 = (buf & 0xFF00) >> 8;	mlxStruct.param.Ks_ta_EE= buf2>127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	/*
	 * Get TGC_EE 0x243C [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.TGC_EE 	= buf2 >127 ? (int8_t)(buf2-256) : (int8_t)buf2;
	mlxStruct.param.TGC 	= (float) mlxStruct.param.TGC_EE/32;

	/*
	 * Get Kv_cp_EE 0x243B [15-8]
	 */
	buf =  i2c_readReg(MLX_EE_Kv_Kta_CP);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.Kv_cp_EE = buf2>127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	/*
	 * Get K_ta_cp_EE 0x243B [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.K_ta_cp_EE = buf2>127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	/*
	 * Get Offset_CP_SP_0 0x243A [9-0]
	 */
	buf =  i2c_readReg(MLX_EE_Off_CP1_2);
	buf2 = buf & 0x03FF;
	mlxStruct.param.Offset_CP_SP_0 = buf2>511 ? (int16_t)(buf2-1024) : (int16_t)buf2;

	/*
	 * Get Offset_CP_SP_1 0x243A [15-10]
	 */
	buf2 = buf & (0xFC00) >> 10;
	mlxStruct.param.Offset_CP_SP_1 = (buf2>31 ? (int16_t)(buf2-64) : (int16_t) buf2)+mlxStruct.param.Offset_CP_SP_0;

	/*
	 * Get Ratio_p1_p2_cp 0x2439 [15-10]
	 */
	buf = i2c_readReg(MLX_EE_ACP_1_2);
	buf2 = (buf & 0xFC00)  >> 10;
	mlxStruct.param.Ratio_p1_p2_cp = buf2>31 ? (int8_t) buf2 -64 : (int8_t) buf2;

	/*
	 * Get alpha_p1_p2_cp 0x2439 [9-0]
	 */
	buf2 = (buf & 0x03FF);
	mlxStruct.param.alp_cp_subp0_EE = buf2;

	/*
	 * Get Res_EE 0x2438 [13-12]
	 */
	buf = i2c_readReg(MLX_EE_Kv_Kt_Scale);
	mlxStruct.param.Res_EE = (uint8_t) ((buf & 0x3000) >> 12);

	/*
	 * Get Kv_scale 0x2438 [11-8]
	 */
	mlxStruct.param.Kv_scale = (uint8_t) ((buf & 0x0F00) >> 8);

	/*
	 * Get K_ta_scale1 0x2438 [7-4]
	 */
	mlxStruct.param.K_ta_scale1 = ((buf & 0x00F0) >> 4) + 8;

	/*
	 * Get K_ta_scale2 0x2438 [3-0]
	 */
	mlxStruct.param.K_ta_scale2 = (uint8_t) buf & 0x000F;

	/*
	 * Get K_ta_rc_EE 0x2437 [15-8]
	 */
	buf =  i2c_readReg(MLX_EE_Kta_avg_2);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.K_ta_Rodd_Ceven = buf2 > 127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	/*
	 * Get K_ta_rc_EE 0x2437 [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.K_ta_Reven_Ceven = buf2 >127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	/*
	 * Get K_ta_rc_EE 0x2436 [15-8]
	 */
	buf = i2c_readReg(MLX_EE_Kta_avg_1);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.K_ta_Rodd_Codd = buf2 > 127 ? (int8_t)(buf2 - 256) : (int8_t)buf2;

	/*
	 * Get K_ta_rc_EE 0x2436 [7-0]
	 */
	buf2 = buf & 0x00FF;
	mlxStruct.param.K_ta_Reven_Codd = buf2 >127 ? (int8_t)(buf2-256) : (int8_t)buf2;

	mlxStruct.param.K_ta_ar[0][0] = mlxStruct.param.K_ta_Rodd_Codd;
	mlxStruct.param.K_ta_ar[0][1] = mlxStruct.param.K_ta_Rodd_Ceven;
	mlxStruct.param.K_ta_ar[1][0] = mlxStruct.param.K_ta_Reven_Codd;
	mlxStruct.param.K_ta_ar[1][1] = mlxStruct.param.K_ta_Reven_Ceven;

	/**
	 * 	Get Kv_avg_Rodd_Codd 0x2434 [15 - 12]
	 */
	buf = i2c_readReg(MLX_EE_Kv_avg);
	buf2 = (buf & 0xF000) >> 12;
	mlxStruct.param.Kv_avg_Rodd_Codd = (buf2 > 7 ? (int8_t) (buf2-16) : (int8_t) buf2);

	/**
	 *  Get Kv_avg_Reven_Codd 0x2434 [11 -  8]
	 */
	buf2 = (buf & 0x0F00) >> 8;
	mlxStruct.param.Kv_avg_Reven_Codd = (buf2 > 7 ? (int8_t) (buf2-16) : (int8_t) buf2);

	/**
	 *  Get Kv_avg_Rodd_Ceven 0x2434 [ 8 -  4]
	 */
	buf2 = (buf & 0x00F0) >> 4;
	mlxStruct.param.Kv_avg_Rodd_Ceven = (buf2 > 7 ? (int8_t) (buf2-16) : (int8_t) buf2);

	/**
	 *  Get Kv_avg_Reven_Ceven 0x2434 [ 3 -  0]
	 */
	buf = (buf & 0x000F);
	mlxStruct.param.Kv_avg_Reven_Ceven = (buf2 > 7 ? (int8_t) (buf2-16) : (int8_t) buf2);

	mlxStruct.param.Kv_ar[0][0] = mlxStruct.param.Kv_avg_Rodd_Codd;
	mlxStruct.param.Kv_ar[0][1] = mlxStruct.param.Kv_avg_Rodd_Ceven;
	mlxStruct.param.Kv_ar[1][0] = mlxStruct.param.Kv_avg_Reven_Codd;
	mlxStruct.param.Kv_ar[1][1] = mlxStruct.param.Kv_avg_Reven_Ceven;

	/*
	 * Get K_Vdd 0x2433 [15-8]
	 */
	buf = i2c_readReg(MLX_EE_Kv_Vdd_25);
	buf2 = (buf & 0xFF00) >> 8;
	mlxStruct.param.K_Vdd = (buf2 > 127 ? (int16_t) (buf2-256) : (int16_t) buf2) << 5;

	/*
	 * Get Vdd_25 0x2433 [7-0]
	 */
	buf2 = (buf & 0x00FF);
	mlxStruct.param.Vdd_25 = (((int16_t) (buf2-256)*32) - (1<<13));

	/*
	 * Get Kv_ptat 0x2432 [15-10]
	 */
	buf = i2c_readReg(MLX_EE_Kv_Kt_ptat);
	buf2 = buf&0xFC00>>10;
	mlxStruct.param.Kv_ptat = (buf2 > 31 ? (float) (buf2-64) : (float) buf2) / 4096.0;

	/*
	 * Get Kt_ptat 0x2432 [9-0]
	 */

	buf2 = buf & 0x03FF;
	mlxStruct.param.Kt_ptat = (buf2 > 511 ? (float) (buf2-1024) : (float) buf2) / 8.0;

	/*
	 * Get V_ptat_25 0x2431 [15-0]
	 */
	buf = i2c_readReg(MLX_EE_PTAT_25);
	mlxStruct.param.V_ptat_25 = buf > 32767 ? (int16_t) (buf-65536) : (int16_t) buf;

	/*
	 * Get GAIN 0x2430 [15-0]
	 */
	buf = i2c_readReg(MLX_EE_GAIN);
	mlxStruct.param.Gain =( buf>32767 ? (int16_t) buf - 65536 : (int16_t) buf);

	/*
	 * Get alp_ref 0x2421 [15-0]
	 */
	buf = i2c_readReg(MLX_EE_Pix_alpha_Avg);
	mlxStruct.param.alp_ref= buf;

	/*
	 * Get alp_scale_cp 0x2420 [15-12]
	 */
	buf = i2c_readReg(MLX_EE_Scale_Acc);
	buf2 = (buf & 0xF000 ) >> 12;
	mlxStruct.param.Acc_scale_cp = buf2 + 27;

	/*
	 * Get alp_scale_row 0x2420 [11-8]
	 */
	mlxStruct.param.Acc_scale_row=  (buf & 0x0F00) >> 8;

	/*
	 * Get alp_scale_row 0x2420 [7-4]
	 */
	mlxStruct.param.Acc_scale_col= (buf & 0x00F0) >> 4;

	/*
	 * Get alp_scale_row 0x2420 [3-0]
	 */
	mlxStruct.param.Acc_scale_rem= buf & 0x00F0;


	/*
	 * Get OCC_cols_values starting from 0x2418
	 */
	getOcc_Col_Vec();

	/*
	 * Get OCC_rows_values starting from 0x2412
	 */
	getOcc_Row_Vec();

	/*
	 * Get Offset_avg 0x2411 [15-0]
	 */
	buf = i2c_readReg(MLX_EE_Pix_Os_Avg);
	mlxStruct.param.Offset_avg =( buf>32767 ? (int16_t) buf - 65536 : (int16_t) buf);

	/*
	 * Get alpha_ptat_EE 0x2410 [15-12]
	 */
	buf = i2c_readReg(MLX_EE_Scale_Occ);
	mlxStruct.param.alpha_ptat_EE = (uint8_t) ((buf & 0xF000)>>12);

	/*
	 * Get OCC_scale_row 0x2410 [11-8]
	 */
	mlxStruct.param.Occ_scale_row = (uint8_t) ((buf & 0x0F00)>>8);

	/*
	 * Get OCC_scale_col 0x2410 [7-4]
	 */
	mlxStruct.param.Occ_scale_col = (uint8_t) ((buf & 0x00F0)>>4);

	/*
	 * Get OCC_scale_rem 0x2410 [3-0]
	 */
	mlxStruct.param.Occ_scale_rem = (uint8_t) buf & 0x000F;

	Calc_Offset_Array();
	Calc_K_ta_Array();
	Calc_Kv_Array();
	mlxStruct.param.Gain_CP_SP_0 = Get16Ringbuffer();
	mlxStruct.param.Gain_CP_SP_1 = Get16Ringbuffer();
}

void getOcc_Row_Vec()
{
	uint8_t buf, buf2;
	for (uint16_t idx =0x2412; idx < 0x2418; idx ++)
	{
		buf = i2c_readReg(idx);
		buf2 = buf & 0x000F;
		mlxStruct.param.OCC_row[(idx-0x2412)*4+0] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x00F0 ) >> 4;
		mlxStruct.param.OCC_row[(idx-0x2412)*4+1] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x0F00 ) >> 8;
		mlxStruct.param.OCC_row[(idx-0x2412)*4+2] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0xF000 ) >> 12;
		mlxStruct.param.OCC_row[(idx-0x2412)*4+3] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));
	}
}

void getOcc_Col_Vec()
{
	uint8_t buf, buf2;
	for (int16_t idx = 0x2418; idx < 0x2420; idx++)
	{
		buf = i2c_readReg(idx);
		buf2 = (buf&0x000F);
		mlxStruct.param.OCC_col[(idx-0x2418)*4+0] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf&0x00F0 >> 4);
		mlxStruct.param.OCC_col[(idx-0x2418)*4+1] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf&0x0F00) >> 8;
		mlxStruct.param.OCC_col[(idx-0x2418)*4+2] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf&0xF000) >> 12;
		mlxStruct.param.OCC_col[(idx-0x2418)*4+3] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));
	}
}

void getAcc_Row_Vec()
{
	uint8_t buf,buf2;
	for (uint16_t idx =0x2422; idx < 0x2428; idx ++)
	{
		buf = i2c_readReg(idx);

		buf2 =  buf & 0x000F;
		mlxStruct.param.ACC_row[(idx-0x2422)*4+0] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x00F0) >> 4;
		mlxStruct.param.ACC_row[(idx-0x2422)*4+1] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x0F00) >> 8;
		mlxStruct.param.ACC_row[(idx-0x2422)*4+2] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0xF000) >> 12;
		mlxStruct.param.ACC_row[(idx-0x2422)*4+3] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));
	}
}

void getAcc_Col_Vec()
{
	uint8_t buf,buf2;
	for (uint16_t idx = 0x2428; idx < 0x2430; idx ++)
	{
		buf = i2c_readReg(idx);

		buf2 = (buf & 0x000F);
		mlxStruct.param.ACC_col[(idx-0x2428)*4+0] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x00F0) >> 4;
		mlxStruct.param.ACC_col[(idx-0x2428)*4+1] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0x0F00) >> 8;
		mlxStruct.param.ACC_col[(idx-0x2428)*4+2] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));

		buf2 = (buf & 0xF000) >> 12;
		mlxStruct.param.ACC_col[(idx-0x2428)*4+3] = (buf2>7 ? ((int8_t) (buf2-15)) : ((int8_t) buf2));
	}
}

void Calc_Offset_Array()
{
	for(int i =0; i< MLX_PIX_HEIGHT; i++)
	{
		for (int j = 0; j< MLX_PIX_WIDTH; j++)
		{
			OS_ref_Array[i][j] = 	mlxStruct.param.Offset_avg + \
								mlxStruct.param.OCC_row[i]*(1<<mlxStruct.param.Occ_scale_row) + \
								mlxStruct.param.OCC_col[j]*(1<<mlxStruct.param.Occ_scale_col) + \
								mlxStruct.param.pixParams[i][j].pixUnion.Offs_pix*(1<<mlxStruct.param.Occ_scale_rem);
		}
	}
}

void Calc_K_ta_Array()
{
	for(int i =0; i< MLX_PIX_HEIGHT; i++)
	{
		for (int j = 0; j< MLX_PIX_WIDTH; j++)
		{
			int8_t Kta_pix = mlxStruct.param.K_ta_ar[(i+1)%2][(j+1)%2];
			K_ta_pix[i][j] = 	(float) (Kta_pix + mlxStruct.param.pixParams[i][j].pixUnion.Kta*(1<<mlxStruct.param.K_ta_scale2)) \
								/ (float)mlxStruct.param.K_ta_scale1;
		}
	}
}

void Calc_Kv_Array()
{
	for(int i =0; i< MLX_PIX_HEIGHT; i++)
	{
		for (int j = 0; j< MLX_PIX_WIDTH; j++)
		{
			//Get right factor
			int8_t K_v_pix = mlxStruct.param.Kv_ar[(i+1)%2][(j+1)%2];
			Kv_pix[i][j] = (float) (K_v_pix / (float) mlxStruct.param.Kv_scale);
		}
	}
}

void Calc_alpha_Array()
{
	for(int i =0; i< MLX_PIX_HEIGHT; i++)
	{
		for (int j = 0; j< MLX_PIX_WIDTH; j++)
		{
			alpha_pix[i][j] = 	(((double) (mlxStruct.param.alp_ref + \
								mlxStruct.param.ACC_row[i]*(1<<mlxStruct.param.Acc_scale_row) + \
								mlxStruct.param.ACC_col[j]*(1<<mlxStruct.param.Acc_scale_col) + \
								mlxStruct.param.pixParams[i][j].pixUnion.alpha*(1<<mlxStruct.param.Acc_scale_rem)))/ (double) (1<<mlxStruct.param.Acc_scale_cp));
		}
	}
}

/* ******************************************* */
/* ***** Begin of Temperature Calculation **** */
/* ******************************************* */

void updateTemp()
{
	//MLX_GET_EEPROM_CONTENT();
	MLX_GET_EEPROM_COEFF();
	//Read Ram Content
	//MLX_GET_RAM_CONTENT();
	//Get corrected Resolution
	MLX_Resolution_Calc();
	//Calculate Supply Voltage
	MLX_SupplyVDD_Calc();
	//Calculate Ambient Temperature
	MLX_AmbTemp_Calc();
	//Gain Compensation
	MLX_Gain_Calc();
	MLX_Gain_Compensation();
	//IR data compensation
	MLX_IR_Data_Compensation();
	//IR data Emissivity compensation

	//IR data gradient compensation
	MLX_GET_CPPIX_GAIN_COMP();
	MLX_CPPIX_DATA_COMP();
	MLX_IR_GRAD_COMP();
	//Normalization to sensitivity
	MLX_Normalize_Sensitvity();
	//Calculation of TO
	MLX_Calc_TO();
}

void MLX_GET_RAM_CONTENT()
{
	//Read in RAM content into rxData
	i2c_readRegBlock(MLX_RAM_PIX_START_ADR, (MLX_RAM_END_ADR-MLX_RAM_PIX_START_ADR + 1));
}

/* ************************************************* *
 * ********* Start of Pixel Data Calculation ******* *
 * ******************* 11.2.2.5 ******************** */

void MLX_Resolution_Calc()
{
	uint16_t ramCont = i2c_readReg(MLX_CTRL_REG_1_ADR);
	ramCont = ((ramCont & 0x0C00) >> 10);

	mlxStruct.param.Res_corr = (1<<mlxStruct.param.Res_EE)/(1<<ramCont);
}

void MLX_SupplyVDD_Calc()
{
	uint16_t uBuf;
	int16_t sBuf;
	uBuf = i2c_readReg(MLX_RAM_VDD_Pix);
	sBuf = uBuf > 32767 ? (int16_t) uBuf - 65536 : (int16_t) uBuf;
	mlxStruct.param.Vdd_Ram = sBuf;

	mlxStruct.param.Vdd = (((float)(mlxStruct.param.Res_corr* sBuf - mlxStruct.param.Vdd_25))/((float)(mlxStruct.param.K_Vdd))) + 3.3;
}

void MLX_AmbTemp_Calc()
{
	uint16_t uBuf;


	//Calculate V_delta
	mlxStruct.param.V_delta = ((float)(mlxStruct.param.Vdd_Ram - mlxStruct.param.Vdd_25)) / mlxStruct.param.K_Vdd;

	//Get V_ptat
	uBuf = i2c_readReg(MLX_RAM_Ta_Ptat);
	mlxStruct.param.V_ptat = uBuf> 32767 ? (int16_t) uBuf - 655536 : (int16_t) uBuf;

	//Get V_be
	uBuf = i2c_readReg(MLX_RAM_Ta_Vbe);
	mlxStruct.param.V_be = uBuf > 32767 ?  (int16_t) (uBuf - 65536) : (int16_t) uBuf;

	//Get alpha_ptat
	mlxStruct.param.alpha_ptat = (uint8_t) (mlxStruct.param.alpha_ptat_EE>>2) + 8;

	//Calculate V_ptat_art
	mlxStruct.param.V_ptat_art = (float)(((float) mlxStruct.param.V_ptat)/ (((float) mlxStruct.param.V_ptat)*mlxStruct.param.alpha_ptat + mlxStruct.param.V_be))*(uint32_t)(1<<18);

	//Calculate T_ambient
	mlxStruct.param.T_ambient = (float)((((mlxStruct.param.V_ptat_art/(1.0 + mlxStruct.param.Kv_ptat*mlxStruct.param.V_delta)) - (float)mlxStruct.param.V_ptat_25))/mlxStruct.param.Kt_ptat) + 25.0;
}

void MLX_Gain_Calc()
{
	uint16_t uBuf;
	int16_t sBuf;

	//Get K_Gain
	uBuf = i2c_readReg(MLX_RAM_Gain);
	sBuf = (uBuf > 32767) ? ((int16_t) uBuf - 65536) : (int16_t) uBuf;
	mlxStruct.param.K_gain = (float) mlxStruct.param.Gain/sBuf;
}

void MLX_Gain_Compensation()
{
	uint16_t buf, buf2;


	buf = i2c_readReg(0x8000);
	buf2 = (i2c_readReg(0x800D) & 0x0380) >> 7;

	if (buf&0x0001) //Subpage 1 has been measured
	{
		MLX_readSP1();
		MLX_setReadFinished(buf);
		HAL_Delay(measPer_ms[buf2]);
		buf = i2c_readReg(0x8000);
		MLX_readSP0();
	}
	else
	{
		MLX_readSP0();
		MLX_setReadFinished(buf);
		HAL_Delay(measPer_ms[buf2]);
		buf = i2c_readReg(0x8000);
		buf2=buf;
		MLX_readSP1();
	}
}

void MLX_readSP0()
{
	uint16_t buf, index;
	int16_t sBuf;
	float k_gain = mlxStruct.param.K_gain;
	//Get raw Pixel Values from Ram and store them properly
	for (int row = 0; row < 24; row = row + 2)
	{
		for(int col = 0 + (row%2); col <32; col = col + 2)
		{
			index = MLX_RAM_PIX_START_ADR + col + row*0x20;
			buf = i2c_readReg(index);
			sBuf = (int16_t) (buf>32767 ? buf-65536 : buf);
			mlxStruct.pixArray[row][col] =  ((float) sBuf) * k_gain;
		}
	}
}

void MLX_readSP1()
{
	uint16_t buf, index;
	int16_t sBuf;
	float k_gain = mlxStruct.param.K_gain;
	//Get raw Pixel Values from Ram and store them properly
	for (int row = 0; row < 24; row = row + 2)
	{
		for(int col = 1-(row%2); col <32; col = col + 2)
		{
			index = MLX_RAM_PIX_START_ADR + col + row*0x20;
			buf = i2c_readReg(index);
			sBuf = (int16_t) (buf>32767 ? buf-65536 : buf);
			mlxStruct.pixArray[row][col] =  ((float) sBuf) * k_gain;
		}
	}
}

void MLX_setReadFinished(uint16_t regCont)
{
	i2c_setReg(0x8000, regCont & 0xFFF7);
}

void MLX_IR_Data_Compensation()
{
	for (int row = 0; row < 24; row++)
	{
		for(int col=0; col <32;  col++)
		{
			mlxStruct.pixArray[row][col] = 	mlxStruct.pixArray[row][col] - \
												((float) OS_ref_Array[row][col] * \
												(1 + K_ta_pix[row][col]*(mlxStruct.param.T_ambient - 25.0)) *\
												(1+ Kv_pix[row][col]*(mlxStruct.param.Vdd - 3.3)));
		}
	}
}

/* ************************************************* *
 * ************ Start of CP Data Calculation ******* *
 * ******************* 11.2.2.6 ******************** */


/*
 * Advice from manufacturer: Filtering useful at this point. E.g. through moving_average
 * Therefore Gain_CP_SP_0/1 are Ringbuffer whose moving average can be accessed through getMvAvg-Method
 */
void MLX_GET_CPPIX_GAIN_COMP()
{
	uint16_t uBuf;
	int16_t sBuf;

	uBuf = i2c_readReg(MLX_RAM_CP_SubP0);
	sBuf = uBuf > 32767 ? (int16_t) uBuf - 65536 : (int16_t) uBuf;
	rb_addValue(&mlxStruct.param.Gain_CP_SP_0, (sBuf * mlxStruct.param.K_gain));

	uBuf = i2c_readReg(MLX_RAM_CP_SubP1);
	sBuf = uBuf > 32767 ? (int16_t) uBuf - 65536 : (int16_t) uBuf;
	rb_addValue(&mlxStruct.param.Gain_CP_SP_1,(sBuf * mlxStruct.param.K_gain));
}

void MLX_CPPIX_DATA_COMP()
{
	mlxStruct.param.K_ta_CP = (float) mlxStruct.param.K_ta_cp_EE/(1<<mlxStruct.param.K_ta_scale1);
	mlxStruct.param.Kv_CP = (float) mlxStruct.param.Kv_cp_EE/(1<<mlxStruct.param.Kv_scale);

	mlxStruct.param.Pix_Offset_CP_SP_0 =	mlxStruct.param.Gain_CP_SP_0.mvgAvg - mlxStruct.param.Offset_CP_SP_0 * \
											(1 + mlxStruct.param.K_ta_CP*(mlxStruct.param.T_ambient-25.0)) * \
											(1 + mlxStruct.param.Kv_CP*(mlxStruct.param.Vdd-3.3));
	mlxStruct.param.Pix_Offset_CP_SP_1 =	mlxStruct.param.Gain_CP_SP_1.mvgAvg - mlxStruct.param.Offset_CP_SP_1 * \
												(1 + mlxStruct.param.K_ta_CP*(mlxStruct.param.T_ambient-25.0)) * \
												(1 + mlxStruct.param.Kv_CP*(mlxStruct.param.Vdd-3.3));
}

/* ************************************************* *
 * ****** Start of IR Gradient Compensation ******** *
 * ******************* 11.2.2.7 ******************** */

uint8_t getPatternChess(uint8_t row, uint8_t column)
{
	if(row%2)return (column+1)%2;
	else return column%2;
}

void MLX_IR_GRAD_COMP()
{
	float offsFact;
	for (int row = 0; row < 24; row++)
	{
		for(int col=0; col <32;  col++)
		{
			if(getPatternChess(row+1, col+1)) 	{offsFact = mlxStruct.param.Pix_Offset_CP_SP_1;}
			else								{offsFact = mlxStruct.param.Pix_Offset_CP_SP_0;}

			mlxStruct.pixArray[row][col] = 	mlxStruct.pixArray[row][col] - ((float)mlxStruct.param.TGC_EE/32.0)*offsFact;
		}
	}
}


/* ************************************************* *
 * ****** Start of Sensitivity Normalizing ********* *
 * ******************* 11.2.2.8 ******************** */

void MLX_Normalize_Sensitvity()
{

	float accFact;

	uint64_t divisor = ((uint64_t)1) <<mlxStruct.param.Acc_scale_cp;
	mlxStruct.param.alp_cp_subp0 = ((double)mlxStruct.param.alp_cp_subp0_EE/((double)divisor));
	mlxStruct.param.alp_cp_subp1 = mlxStruct.param.alp_cp_subp0*(1.0 + (double)(mlxStruct.param.Ratio_p1_p2_cp/(1<<7)));
	mlxStruct.param.Ks_ta = ((double)mlxStruct.param.Ks_ta_EE)/(1<<13);

	for (int row = 0; row < 24; row++)
	{
		for(int col=0; col <32;  col++)
		{
			if(getPatternChess(row+1, col+1)) 	{accFact = mlxStruct.param.alp_cp_subp0;}
			else								{accFact = mlxStruct.param.alp_cp_subp1;}
			alpha_pix[row][col] = (double)(alpha_pix[row][col]-(double)mlxStruct.param.TGC*accFact)*(double)(1.0 + mlxStruct.param.Ks_ta*((double)mlxStruct.param.T_ambient-25.0));
		}
	}
}

//ToDo: convert import numbers to double
void MLX_Calc_TO()
{
	double alp_buf;

	mlxStruct.param.Ks_To2 = (((double)mlxStruct.param.Ks_to2_EE)/((double)(1<<(mlxStruct.param.Ks_scale_to + 8))));
	mlxStruct.param.T_ak4 = pow(mlxStruct.param.T_ambient + 273.15, 4);
	mlxStruct.param.T_rk4 = pow(mlxStruct.param.T_ambient + 265.15, 4);

	for (int row = 0; row < 24; row++)
	{
		for(int col=0; col <32;  col++)
		{
			alp_buf = pow(alpha_pix[row][col], 3);
			double buff = (alp_buf*(double)mlxStruct.pixArray[row][col]);
			buff += (alp_buf*((double) alpha_pix[row][col])*mlxStruct.param.T_ak4);
			for(int i=0; i<4; i++)
			{
				buff= sqrt(buff);
			}
			S_pix[row][col] = mlxStruct.param.Ks_To2*buff;
		}
	}
	for (int row = 0; row < 24; row++)
	{
		for(int col=0; col <32;  col++)
		{
			extInterface.tempMap[row][col] = (float)pow((mlxStruct.pixArray[row][col]/(alpha_pix[row][col]*(1-mlxStruct.param.Ks_To2*273.15)+S_pix[row][col]))+mlxStruct.param.T_ak4,1.0/4.0)-273.15;
		}
	}
}

