/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"
#include "spline.h"
#include "ringbuff.h"

#include <gsl/gsl_math.h>
#include <gsl/gsl_interp2d.h>
#include <gsl/gsl_spline2d.h>

/*

#include "MLX90640_I2C.h"

MLX_RXDATA rxData;
uint8_t *pRxData = &rxData.rx8Data[0];
uint16_t *pRx16Data = &rxData.rx16Data[0];
uint8_t txData[2];
uint8_t *pTxData = &txData[0];
*/


#include <MLX90640_API.h>
#include <MLX90640_I2C_Driver.h>
paramsMLX90640 parMLX;
paramsMLX90640 *pParam = &parMLX;
uint16_t frData[834];
uint16_t *pFrData = &frData[0];
uint16_t EEData[834];
uint16_t *pEEData = &EEData[0];
uint16_t colorMap[187];
float resTemp[834];
RingBuffer irDataCP0;
RingBuffer irDataCP1;



// ----------------------------------------------------------------------------
//
// Standalone STM32F4 led blink sample (trace via ITM).
//
// In debug configurations, demonstrate how to print a greeting message
// on the trace device. In release configurations the message is
// simply discarded.
//
// Then demonstrates how to blink a led with 1 Hz, using a
// continuous loop and SysTick delays.
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the ITM output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)



/* USART configuration structure for USART2 */
USART_HandleTypeDef usart2_struct;


struct heatmap
{
	uint16_t tempVal[24][32];
	float maxVal;
	float minVal;
};

struct heatmap HeatMap;

//extern HAL_StatusTypeDef HAL_USART_Init(USART_HandleTypeDef* handler);

void displayHeatMap(void);
void getMaxVal(void);
void getMinVal(void);
std::vector<uint16_t> parseTempVal(std::vector<double> inVal);
void generateColorMap(void);
void ili9341_DrawHLineColored( std::vector<uint16_t> RGB_Code, uint16_t Ypos, uint16_t Length );
void ili9341_DrawVLineColored( std::vector<uint16_t> RGB_Code, uint16_t Xpos, uint16_t Length );
void initLED(void);

void USART2_Init(void);


// ----- main() ---------------------------------------------------------------



int main(void)
{
	HAL_Init();
	//timer_start();
	//blink_led_init();

	USART2_Init();

	irDataCP0 = Get16Ringbuffer();
	irDataCP1 = Get16Ringbuffer();
	ili9341_Init();
	HAL_Delay(100);
	
	// Initially display green screen 
	ili9341_DrawRect( 0x0EE0, 0x0000, 0x0000, 0x0140, 0x00F0);
	initLED();
	generateColorMap();
	
	// Initialize IR-Camera sensor	
	MLX90640_I2CInit();
	MLX90640_SetChessMode(0x33);
	MLX90640_SetRefreshRate(0x33, Eight_Hertz);
	MLX90640_SetSubPageRepeat(subPageRepeat_Enable);	
	MLX90640_DumpEE((uint8_t) 0x33, pEEData);
	MLX90640_ExtractParameters(pEEData,pParam);
	// Infinite loop


  while (1)
    {
      blink_led_on();
      //timer_sleep(seconds == 0 ? TIMER_FREQUENCY_HZ : BLINK_ON_TICKS);
      MLX90640_GetFrameData(0x33, pFrData);
      MLX90640_CalculateTo(pFrData, pParam, 1.0, 22.0, &resTemp[0]);
      //blink_led_off();
      //timer_sleep(BLINK_OFF_TICKS);
      //updateTemp();
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,  GPIO_PIN_SET);
      displayHeatMap();
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5,  GPIO_PIN_RESET);
    }
  // Infinite loop, never return.
}

void USART2_Init(void)
{

    /* Bit configuration structure for GPIOA PIN9 and PIN10 */
    GPIO_InitTypeDef gpioa_init_struct;

    /* Enable clock for UART2, AFIO and GPIOA */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    /* GPIOA PIN2 alternative function Tx */
    gpioa_init_struct.Pin = GPIO_PIN_2;
    gpioa_init_struct.Speed = GPIO_SPEED_HIGH;
    gpioa_init_struct.Mode = GPIO_MODE_AF_PP;
    gpioa_init_struct.Alternate = GPIO_AF7_USART2;
    gpioa_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpioa_init_struct);
    /* GPIOA PIN3 alternative function Rx */
    gpioa_init_struct.Pin = GPIO_PIN_3;
    gpioa_init_struct.Speed = GPIO_SPEED_HIGH;
    gpioa_init_struct.Mode = GPIO_MODE_AF_PP;
    gpioa_init_struct.Alternate = GPIO_AF7_USART2;
    gpioa_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpioa_init_struct);


    /* Baud rate 9600, 8-bit data, One stop bit
     * No parity, Do both Rx and Tx, No HW flow control
     */
    usart2_struct.Instance = USART2;

    usart2_struct.Init.BaudRate = 115200;
    usart2_struct.Init.WordLength = UART_WORDLENGTH_8B;
    usart2_struct.Init.StopBits = UART_STOPBITS_1;
    usart2_struct.Init.Parity = UART_PARITY_NONE;
    usart2_struct.Init.Mode = UART_MODE_TX_RX;
    HAL_USART_Init(&usart2_struct);
}

void initLED(void)
{
    GPIO_InitTypeDef gpioc_init_struct;

    /* Enable clock for UART2, AFIO and GPIOA */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* GPIOA PIN2 alternative function Tx */
    gpioc_init_struct.Pin = GPIO_PIN_5;
    gpioc_init_struct.Speed = GPIO_SPEED_HIGH;
    gpioc_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    //gpioc_init_struct.Alternate = GPIO_;
    gpioc_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &gpioc_init_struct);
}
/*
void displayHeatMap()
{
	getMaxVal();
	getMinVal();

	//parseTempVal();
	std::vector<std::vector<double>> vec(24);
	std::vector<double> vecStz(32), vecVal(32), vecOut(32*2);
	std::vector<uint16_t> printVec;
	for(int row = 0; row<24; row++)
	{
		/*
		for(int col = 0; col<32; col++)
		{
			ili9341_DrawRect( (uint16_t) HeatMap.tempVal[row][col], col*10, row*10, 10, 10);
		}*/
/*
		for(int i  = 0 ; i<32; i++)
		{
			vecStz[i] = (double)(i*2.0);
		}
		for(int i  = 0 ; i<32; i++)
		{
			vecVal[i] = resTemp[row*32 + i];
		}
		tk::spline s;
		s.set_points(vecStz,vecVal);
		for(int i=0; i<64; i++)
		{
			vecOut[i] = s(i);
		}
		vec[row] = vecOut;
	}
	vecStz.clear();
	vecVal.clear();
	vecOut.clear();
	printVec.clear();
	tk::spline s;
	std::vector<double> vecStz_v(24), vecVal_v(24), vecOut_v(48);
	std::vector<uint16_t> printVec_v;
	for (int col = 0; col<64; col++)
	{
		for(int i  = 0 ; i<24; i++)
		{
			vecVal_v[i] = vec[i][col];
		}
		for(int i  = 0 ; i<24; i++)
		{
			vecStz_v[i] = (double)(i*2.0);
		}
		s.set_points(vecStz_v,vecVal_v);
		for(int i=0; i<64; i++)
		{
			vecOut_v[i] = s(i);
		}
		printVec = parseTempVal(vecOut_v);
		ili9341_DrawVLineColored(printVec_v, col, 48);
	}
	vec.clear();
}
*/

void setGridValues(gsl_spline2d* spline, double *za, std::vector<uint16_t> xVec, std::vector<uint16_t> yVec)
{
  
    for (int row =0; row < yVec.length(); row++)
    {
      for (int col=0; col < xVec.length(); col++)
      {
	gsl_spline2d_set(spline, za, xVec(col), yVec(row), resTemp[row*32 + col]);
      }
    } 
}

void displayHeatMap()
{
	getMaxVal();
	getMinVal();

	std::vector<std::vector<double>> vec(24);
	std::vector<uint16_t> bufVec; //Stz = Stützstellen; Val = Sensorwerte; Out =
	std::vector<std::vector<uint16_t>> printArr;
	std::vector<uint16_t> xVec, yVec; //xVec absciss vector. yVec ordinated Vector

	//define abscisse vector
	for (int i=0; i <32; i = i++)
	{
		xVec[i] = 10*i;
	}
	//define ordinate vector
	for (int i=0; i <24; i = i++)
	{
		yVec[i] = 10*i;
	}

	// Example from https://www.gnu.org/software/gsl/doc/html/interp.html#id1
	  const gsl_interp2d_type *T = gsl_interp2d_bilinear;
	  const size_t N = 10;             /* number of points to interpolate */
	  const double xa[] = { 0.0, 310.0 }; /* define unit square */
	  const double ya[] = { 0.0, 230.0 };
	  const size_t nx = sizeof(xa) / sizeof(double); /* x grid points */
	  const size_t ny = sizeof(ya) / sizeof(double); /* y grid points */
	  double *za = malloc(nx * ny * sizeof(double));
	  gsl_spline2d *spline = gsl_spline2d_alloc(T, nx, ny);
	  gsl_interp_accel *xacc = gsl_interp_accel_alloc();
	  gsl_interp_accel *yacc = gsl_interp_accel_alloc();
	  size_t i, j;

	  /* set z grid values */
	  setGridValues(spline, za, xVec, yVec);

	  /* initialize interpolation */
	  gsl_spline2d_init(spline, xa, ya, za, nx, ny);

	for (int row=0; row <24; i = i++)
	{
	    for (int col = 0; col <32; col++)
	    {
	  /* interpolate N values in x and y and print out grid for plotting */
	      for (int i = 0; i < N; ++i)
		{
		  double xi = i / (N - 1.0);

		  for (j = 0; j < N; ++j)
		  {
		    double yj = j / (N - 1.0);
		    double zij = gsl_spline2d_eval(spline, xi, yj, xacc, yacc);
		  }
		}
	    }
	}
	gsl_spline2d_free(spline);
	gsl_interp_accel_free(xacc);
	gsl_interp_accel_free(yacc);
	free(za);


	printArr = parseTempVal(vecOut_v);
	ili9341_DrawVLineColored(printVec_v, col, 48);
	vec.clear();
}

void getMaxVal()
{
	float max = -9999.0;
	for(int i=0; i< 768; i++)
	{
		if(resTemp[i]>max) max = resTemp[i];
	}
	HeatMap.maxVal = (uint16_t) max;
}

void getMinVal()
{
	float min = 9999.0;
	for(int i=0; i< 768; i++)
	{
		if(resTemp[i]<min) min = resTemp[i];
	}
	HeatMap.minVal = (uint16_t) min;
}

std::vector<uint16_t> parseTempVal(std::vector<double> inVal)
{
	float range = HeatMap.maxVal-HeatMap.minVal;
	std::vector<uint16_t> retVec(inVal.size());
	for (int i = 0; i<inVal.size(); i++)
	{
		//retVec[i] = colorMap[(uint16_t)(((resTemp[i*32 + j]-HeatMap.minVal)/range)*219)];
		retVec[i] = colorMap[(uint16_t)(((inVal[i]-HeatMap.minVal)/range)*219)];
	}
	return retVec;
}

void generateColorMap()
{
	for (int i=0; i<32; i++) //Alle Blauen nacheinander auffüllen --> 32 Indizes
	{
		colorMap[i]=i;
	}
	for (int j = 32; j<95; j++) //Hinzukommenden alle roten auffüllen --> 31 Indizes, da 0x001F nicht mitgezählt
	{
		colorMap[j] = 0x001F + (j-31)*32;
	}
	for (int j = 95; j<126; j++) //Nun werden blaue wieder abgezogen --> 31 Indizes, da 0xF81F nicht mitgezählt
	{
			colorMap[j] = 0x085D - j;
	}
	for (int j = 126; j<156; j++) //Nun kommen grüne dazu --> 63 Indizes, da 0xF800 nicht mitgezählt
	{
			colorMap[j] = 0x07E0 +((j-125)<<11);
	}
	for (int j = 157; j<219; j++) //Nun final die blauen dazu --> 31 Indizes, da 0xFFE0 nicht mitgezählt
	{
			colorMap[j] = 0xFFE0 - (j-156);
	}
}


void ili9341_DrawHLineColored( std::vector<uint16_t> RGB_Code, uint16_t Ypos, uint16_t Length )
{
  uint16_t i = 0;
  uint16_t buffer[ILI9341_LCD_PIXEL_WIDTH];

  /* Set Cursor */
  ili9341_SetDisplayWindow(0, Ypos, Length, 1 );


  for( i = 0; i < Length; i++ )
  {
    /* Write 16-bit GRAM Reg */
    buffer[i] = RGB_Code[i];
  }
  LCD_IO_WriteBuffer16(buffer,Length);
}
void ili9341_DrawVLineColored( std::vector<uint16_t> RGB_Code, uint16_t Xpos, uint16_t Length )
{
  uint16_t i = 0;
  uint16_t buffer[ILI9341_LCD_PIXEL_WIDTH];

  /* Set Cursor */
  ili9341_SetDisplayWindow(Xpos, 0, 0, Length );


  for( i = 0; i < Length; i++ )
  {
    /* Write 16-bit GRAM Reg */
    buffer[i] = RGB_Code[i];
  }
  LCD_IO_WriteBuffer16(buffer,Length);
}
// ----------------------------------------------------------------------------
