/*
 * MXL90640_IC.h
 *
 *  Created on: 26.01.2020
 *      Author: masltov
 */

#ifndef INCLUDE_MXL90640_IC_H_
#define INCLUDE_MXL90640_IC_H_

#include "ringbuff.h"

// --- I2C Bus related defines ---//
#define IR_INT_PIN           GPIO_PIN_5
#define IR_INT_GPIO_PORT     GPIOB
#define IR_INT_CLK_ENABLE()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define IR_INT_CLK_DISABLE() __HAL_RCC_GPIOB_CLK_DISABLE()

#define MLX_SLAVE_ADR		0x33 	//I2C Adress of MLX90640

#define MLX_PIX_HEIGHT		24
#define MLX_PIX_WIDTH		32

#define MLX_RAM_PIX_START_ADR 	0x0400 	//Start address of MLX90640 RAM
#define MLX_RAM_PIX_END_ADR 	0x06FF	//End address of MLX90640 RAM Pixel
#define MLX_RAM_END_ADR 		0x073F	//End address of MLX90640 RAM Pixel
#define MLX_RAM_CTRL_OFF		0x0700

#define MLX_EEPROM_ADR_STA 	0x2400
#define MLX_PIX_PARAM_OFF	0x2440
#define MLX_EEPROM_ADR_END	0x273F

#define MLX_CTRL_REG_1_ADR	0x800D

#define MLX_RAM_Ta_Vbe		MLX_RAM_CTRL_OFF
#define MLX_RAM_CP_SubP0	MLX_RAM_CTRL_OFF + 8
#define MLX_RAM_Gain		MLX_RAM_CTRL_OFF + 0xA
#define MLX_RAM_Ta_Ptat		MLX_RAM_CTRL_OFF + 0x20
#define MLX_RAM_CP_SubP1	MLX_RAM_CTRL_OFF + 0x28
#define MLX_RAM_VDD_Pix		MLX_RAM_CTRL_OFF + 0x2A

#define MLX_EE_Osc_Trim 		MLX_EEPROM_ADR_STA + 0
#define MLX_EE_Ana_Trim 		MLX_EEPROM_ADR_STA + 1
#define MLX_EE_Conf_Reg 		MLX_EEPROM_ADR_STA + 3
#define MLX_EE_ID1_Reg 			MLX_EEPROM_ADR_STA + 7
#define MLX_EE_ID2_Reg 			MLX_EEPROM_ADR_STA + 8
#define MLX_EE_ID3_Reg 			MLX_EEPROM_ADR_STA + 9
#define MLX_EE_CTR1_Reg 		MLX_EEPROM_ADR_STA + 0xC
#define MLX_EE_CTR2_Reg 		MLX_EEPROM_ADR_STA + 0xD
#define MLX_EE_I2C_Conf 		MLX_EEPROM_ADR_STA + 0xE
#define MLX_EE_I2C_Add 			MLX_EEPROM_ADR_STA + 0xF
#define MLX_EE_Scale_Occ 		MLX_EEPROM_ADR_STA + 0x10
#define MLX_EE_Pix_Os_Avg 		MLX_EEPROM_ADR_STA + 0x11
#define MLX_EE_Pix_Occ_Row		MLX_EEPROM_ADR_STA + 0x12
#define MLX_EE_Pix_Occ_Col		MLX_EEPROM_ADR_STA + 0x18
#define MLX_EE_Scale_Acc 		MLX_EEPROM_ADR_STA + 0x20
#define MLX_EE_Pix_alpha_Avg 	MLX_EEPROM_ADR_STA + 0x21
#define MLX_EE_Pix_Acc_Row		MLX_EEPROM_ADR_STA + 0x22
#define MLX_EE_Pix_Acc_Col		MLX_EEPROM_ADR_STA + 0x28
#define MLX_EE_GAIN				MLX_EEPROM_ADR_STA + 0x30
#define MLX_EE_PTAT_25			MLX_EEPROM_ADR_STA + 0x31
#define MLX_EE_Kv_Kt_ptat		MLX_EEPROM_ADR_STA + 0x32
#define MLX_EE_Kv_Vdd_25		MLX_EEPROM_ADR_STA + 0x33
#define MLX_EE_Kv_avg			MLX_EEPROM_ADR_STA + 0x34
#define MLX_EE_Kta_avg_1		MLX_EEPROM_ADR_STA + 0x36
#define MLX_EE_Kta_avg_2		MLX_EEPROM_ADR_STA + 0x37
#define MLX_EE_Kv_Kt_Scale		MLX_EEPROM_ADR_STA + 0x38
#define MLX_EE_ACP_1_2			MLX_EEPROM_ADR_STA + 0x39
#define MLX_EE_Off_CP1_2		MLX_EEPROM_ADR_STA + 0x3A
#define MLX_EE_Kv_Kta_CP		MLX_EEPROM_ADR_STA + 0x3B
#define MLX_EE_KsTa_TGC			MLX_EEPROM_ADR_STA + 0x3C
#define MLX_EE_Ks_To_2_1		MLX_EEPROM_ADR_STA + 0x3D
#define MLX_EE_Ks_To_4_3		MLX_EEPROM_ADR_STA + 0x3E
#define MLX_EE_CT				MLX_EEPROM_ADR_STA + 0x3F


// ToDo Add all the other Registers in EEPROM in future, currently there's no demand

#define MLX_GET_OCC_ROW_ADR( ROW_NUM ) 	MLX_EE_Pix_Occ_Row + ((ROW_NUM > 0 && ROW_NUM < 25) ? ((ROW_NUM - (ROW_NUM % 4))/4) : 0)
#define MLX_GET_OCC_COL_ADR( COL_NUM ) 	MLX_EE_Pix_Occ_Col + ((COL_NUM > 0 && COL_NUM < 33) ? ((COL_NUM - (COL_NUM % 4))/4) : 0)
#define MLX_GET_ACC_ROW_ADR( ROW_NUM ) 	MLX_EE_Pix_Acc_Row + ((ROW_NUM > 0 && ROW_NUM < 25) ? ((ROW_NUM - (ROW_NUM % 4))/4) : 0)
#define MLX_GET_ACC_COL_ADR( COL_NUM ) 	MLX_EE_Pix_Acc_Col + ((COL_NUM > 0 && COL_NUM < 33) ? ((COL_NUM - (COL_NUM % 4))/4) : 0)

#define MLX_GET_PIX_RAM_ADR( ROW_NUM, COL_NUM )		(MLX_RAM_PIX_START_ADR + ((ROW_NUM-1) * 0x0020 + COL_NUM-1))
#define MLX_GET_PIX_PARAM_OFF( ROW_NUM, COL_NUM )	(MLX_PIX_PARAM_OFF + ((ROW_NUM-1) * 0x0020 + COL_NUM-1))

struct pixParam
{
	unsigned int outlier 	: 1;
	signed 	 int Kta	 	: 3;
	signed 	 int alpha		: 6;
	signed 	 int Offs_pix	: 6;
};

typedef union pixParamUn
{
	uint8_t 			doubleWord[2];
	uint16_t			word;
	struct 	pixParam 	pixUnion;
}pixPARAM;

typedef struct mlx_param
 {
	/**************** Params from EEPROM ***********************/

	pixPARAM 	pixParams[24][32];	// [15 -  0] 	of 	0x2440 - 0x2473F
	uint8_t		Step;				// [13 - 12] 	of 	0x243F
	uint8_t		CT4;				// [11 -  8] 	of 	0x243F
	uint8_t		CT3;				// [ 7 -  4] 	of 	0x243F
	uint8_t		Ks_scale_to;		// [ 3 -  0] 	of 	0x243F
	int8_t		Ks_to4_EE;			// [15 -  8] 	of 	0x243E
	int8_t		Ks_to3_EE;			// [ 7 -  0] 	of 	0x243E
	int8_t		Ks_to2_EE;			// [15 -  8] 	of 	0x243D
	int8_t		Ks_to1_EE;			// [ 7 -  0] 	of 	0x243D
	int8_t		Ks_ta_EE;			// [15 -  8] 	of 	0x243C
	int8_t		TGC_EE;				// [ 7 -  0] 	of 	0x243C
	int8_t 		Kv_cp_EE;			// [15 -  8] 	of 	0x243B
	int8_t		K_ta_cp_EE;			// [ 7 -  0] 	of 	0x243B
	int16_t		Offset_CP_SP_1;		// [15 - 10] 	of 	0x243A
	int16_t		Offset_CP_SP_0; 	// [ 9 -  0] 	of 	0x243A
	int8_t		Ratio_p1_p2_cp;		// [15 - 10]	of	0x2439
	uint16_t	alp_cp_subp0_EE;	// [ 9 -  0]	of	0x2439
	uint8_t 	Res_EE; 			// [13 - 12] 	of 	0x2438
	uint8_t 	Kv_scale; 			// [11 -  8] 	of 	0x2438
	uint8_t 	K_ta_scale1;		// [ 7 -  4] 	of 	0x2438
	uint8_t 	K_ta_scale2;		// [ 3 -  0] 	of 	0x2438
	int8_t 		K_ta_Rodd_Ceven;	// [15 -  8] 	of 	0x2437
	int8_t 		K_ta_Reven_Ceven;	// [ 7 -  0] 	of 	0x2437
	int8_t 		K_ta_Rodd_Codd;		// [15 -  8] 	of 	0x2436
	int8_t 		K_ta_Reven_Codd;	// [ 7 -  0] 	of 	0x2436
	int8_t		Kv_avg_Rodd_Codd;	// [15 - 12] 	of 	0x2434
	int8_t		Kv_avg_Reven_Codd;	// [11 -  8] 	of 	0x2434
	int8_t		Kv_avg_Rodd_Ceven;	// [ 8 -  4] 	of 	0x2434
	int8_t		Kv_avg_Reven_Ceven;	// [ 3 -  0] 	of 	0x2434
	int16_t		K_Vdd;				// [15 -  8] 	of 	0x2433
	int16_t		Vdd_25; 			// [ 7 -  0] 	of 	0x2433
	float		Kv_ptat;			// [15 - 10]	of 	0x2432
	float		Kt_ptat;			// [ 9 -  0]	of 	0x2432
	int16_t 	V_ptat_25;			// [15 -  0]   	of 	0x2431
	int16_t 	Gain;				// [15 -  0] 	of 	0x2430
	int8_t		ACC_col[32];		// [15 -  0] 	of 	0x2428 - 0x2429
	int8_t 		ACC_row[24];		// [15 -  0] 	of 	0x2422 - 0x2427
	uint16_t	alp_ref;			// [15 -  0]	of 	0x2421
	uint8_t		Acc_scale_cp;		// [15 - 12] 	of	0x2420
	uint8_t		Acc_scale_row;		// [11 -  8] 	of	0x2420
	uint8_t		Acc_scale_col;		// [ 7 -  4] 	of	0x2420
	uint8_t		Acc_scale_rem;		// [ 3 -  0] 	of	0x2420
	int8_t		OCC_col[32];		// [15 -  0] 	of 	0x2418 - 0x2419
	int8_t 		OCC_row[24];		// [15 -  0] 	of 	0x2412 - 0x2417
	int16_t		Offset_avg;			// [15 -  0] 	of 	0x2411
	uint8_t		alpha_ptat_EE; 		// [15 - 12]	of 	0x2410
	uint8_t 	Occ_scale_row;		// [11 -  8] 	of 	0x2410
	uint8_t 	Occ_scale_col;		// [ 7 -  4] 	of 	0x2410
	uint8_t 	Occ_scale_rem;		// [ 3 -  0] 	of 	0x2410

	/***************** Calculated Params *********************/

	//********** Resolution Corection **********//
	uint8_t 	Res_corr;

	//********** Supply Voltage **********//
	float 		Vdd;
	int16_t 	Vdd_Ram; 			//Row value of RAM @0x072A

	//********** Ambient Temperature **********//
	float		V_delta;
	int16_t 	V_ptat;
	int16_t 	V_be;
	uint8_t		alpha_ptat;
	float	 	V_ptat_art;
	float 		T_ambient;

	//********** Gain Calculation **********//
	float		K_gain;


	//********** IR Data Compensation **********//
	int8_t 		K_ta_ar[2][2]; 	//Get correct K_ta value by [row%2][col%2]
	int8_t 		Kv_ar[2][2];	//Get correct Kv value by [row%2][col%2]
	float 		TGC;

	//********** CP Gain Compensation **********//
	RingBuffer		Gain_CP_SP_0;
	RingBuffer		Gain_CP_SP_1;

	//********** CP Data Compensation **********//
	float 		K_ta_CP;
	float		Kv_CP;
	float		Pix_Offset_CP_SP_0;
	float		Pix_Offset_CP_SP_1;

	double		alp_cp_subp0;
	double 		alp_cp_subp1;
	double		Ks_ta;

	double		Ks_To2;
	double		T_ak4;
	double		T_rk4;


 } MLX_PARAM;


/*****************************************
 * START OF MLX90640 STATUS REGISTER     *
 * ***************************************/

// --- Status Register Struct --- //
typedef struct mtx_status_Reg{
	unsigned int res_1 		: 11;
	unsigned int enOvWrite 	: 1;
	unsigned int newDtAvai	: 1;
	unsigned int ltSubPgRd	: 3;
}MLX_ST_STR;

// --- Bit Masks --- //
#define OVERWRITE_EN_Msk 	0x0010
#define DATAAVAILABLE_Msk 	0x0008
#define LAST_SUBPG_RX_Msk 	0x0007



/*****************************************
 * START OF MLX90640 CONTROL REGISTER 1  *
 * ***************************************/

// --- Control Register Struct --- //
typedef struct mtx_ctrl_Reg{
	unsigned int res_1 		: 3;
	unsigned int readPat 	: 1;
	unsigned int resolution	: 2;
	unsigned int refrRate	: 3;
	unsigned int selSubPg	: 3;
	unsigned int enSubPgRep	: 1;
	unsigned int enDtHold	: 1;
	unsigned int res_2		: 1;
	unsigned int enSubPgMo	: 1;
}MLX_CTRL_STR;

// --- Bit Masks --- //
#define READ_PAT_Msk 		0x1000
#define RESOLUTION_Msk 		0x0D00
#define REFR_RATE_Msk 		0x0380
#define SUBPG_SEL_Msk 		0x0070
#define SUBPG_REP_EN_Msk	0x0008
#define DATA_HOLD_EN_Msk 	0x0004
#define SUBPG_MODE_EN_Msk 	0x0001

// -- Offsets --- /
#define READ_PAT_Off 		12
#define RESOLUTION_Off 		10
#define REFR_RATE_Off 		7
#define SUBPG_SEL_Off 		4
#define SUBPG_REP_EN_Off	3
#define DATA_HOLD_EN_Off 	2
#define SUBPG_MODE_EN_Off 	0

// --- enums --- //
enum
{
	TV_INTERLEAVE 	= 0,
	CHESS_PAT		= 1
};

enum
{
	HALF_HERTZ		= 0,
	ONE_HERTZ		= 1,
	TWO_HERTZ		= 2,
	FOUR_HERTZ		= 3,
	EIGHT_HERTZ		= 4,
	SIXTEEN_HERTZ	= 5,
	THIRTYTWO_HERTZ	= 6,
	SIXTYFOUR_HERTZ	= 7
};

uint16_t measPer_ms[8] = {2000, 1000, 500, 250, 125, 68, 32, 16};

enum
{
	SUBPAGE_ZERO		= 0,
	SUBPAGE_ONE			= 1,
};

#endif /* INCLUDE_MXL90640_IC_H_ */
