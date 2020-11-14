/*
 * Communication.c
 *
 *  Created on: 19.01.2020
 *      Author: masltov
 */
#include <main.h>

struct comInterface interface;

#ifdef HAL_SPI_MODULE_ENABLED
SPI_HandleTypeDef hbus_spi1;
SPI_HandleTypeDef hbus_spi2;
SPI_HandleTypeDef hbus_spi3;

//static uint32_t SPI1InitCounter = 0;
static uint32_t SPI2InitCounter = 0;
//static uint32_t SPI3InitCounter = 0;
#endif /* HAL_SPI_MODULE_ENABLED */

static uint32_t SPI_GetPrescaler(uint32_t clock_src_hz, uint32_t baudrate_mbps)
{
  uint32_t divisor = 0;
  uint32_t spi_clk = clock_src_hz;
  uint32_t presc = 0;
  static const uint32_t baudrate[]= {
                                      SPI_BAUDRATEPRESCALER_2,
                                      SPI_BAUDRATEPRESCALER_4,
                                      SPI_BAUDRATEPRESCALER_8,
                                      SPI_BAUDRATEPRESCALER_16,
                                      SPI_BAUDRATEPRESCALER_32,
                                      SPI_BAUDRATEPRESCALER_64,
                                      SPI_BAUDRATEPRESCALER_128,
                                      SPI_BAUDRATEPRESCALER_256,
                                    };

  while(spi_clk > baudrate_mbps)
  {
    presc = baudrate[divisor];

    if (++divisor > 7)
    {
      break;
    }

    spi_clk = (spi_clk >> 1);
  }

  return presc;
}

/**
  * @brief  SPI2 Bus initialization.
  */
__weak HAL_StatusTypeDef MX_SPI2_Init(SPI_HandleTypeDef *hspi, uint32_t baudrate_presc, uint32_t DataSize)
{
  HAL_StatusTypeDef ret = HAL_OK;

  hspi->Init.BaudRatePrescaler = baudrate_presc;
  hspi->Init.Direction         = SPI_DIRECTION_1LINE;
  hspi->Init.CLKPhase          = SPI_PHASE_1EDGE;
  hspi->Init.CLKPolarity       = SPI_POLARITY_LOW;
  hspi->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  hspi->Init.CRCPolynomial     = 7;
  hspi->Init.DataSize          = DataSize;
  hspi->Init.FirstBit          = SPI_FIRSTBIT_MSB;
  hspi->Init.NSS               = SPI_NSS_SOFT;
  hspi->Init.TIMode            = SPI_TIMODE_DISABLE;
  hspi->Init.Mode              = SPI_MODE_MASTER;

  if (HAL_SPI_Init(hspi) != HAL_OK)
  {
    ret = HAL_ERROR;
  }

  return ret;
}

static void SPI2_MspInit()
{
  GPIO_InitTypeDef  gpio_init;

  /*** Configure the GPIOs ***/
  /* Enable GPIO clock */
  BUS_SPI2_SCK_GPIO_CLK_ENABLE();
  BUS_SPI2_NSS_GPIO_CLK_ENABLE();
  BUS_SPI2_MISO_GPIO_CLK_ENABLE();
  BUS_SPI2_MOSI_GPIO_CLK_ENABLE();

  /* Configure SPI NSS */
  gpio_init.Pin   = BUS_SPI2_NSS_GPIO_PIN;
  gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
  gpio_init.Pull  = GPIO_NOPULL;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(BUS_SPI2_NSS_GPIO_PORT, &gpio_init);

  /* Configure SPI SCK */
  gpio_init.Pin       = BUS_SPI2_SCK_GPIO_PIN;
  gpio_init.Mode      = GPIO_MODE_AF_PP;
  gpio_init.Pull      = GPIO_NOPULL;
  gpio_init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = BUS_SPI2_SCK_GPIO_AF;
  HAL_GPIO_Init(BUS_SPI2_SCK_GPIO_PORT, &gpio_init);

  /* Configure SPI MOSI */
  gpio_init.Pin       = BUS_SPI2_MOSI_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI2_MOSI_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI2_MOSI_GPIO_PORT, &gpio_init);

  /* Configure SPI MISO */
  gpio_init.Pin       = BUS_SPI2_MISO_GPIO_PIN;
  gpio_init.Alternate = BUS_SPI2_MISO_GPIO_AF;
  gpio_init.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BUS_SPI2_MISO_GPIO_PORT, &gpio_init);

  /*** Configure the SPI peripheral ***/
  /* Enable SPI clock */
  BUS_SPI2_CLK_ENABLE();
}

int32_t SPI2_Init(void)
{
  int32_t ret = 0;
  hbus_spi2.Instance = SPI2;
  if (SPI2InitCounter++ == 0)
  {
    if (HAL_SPI_GetState(&hbus_spi2) == HAL_SPI_STATE_RESET)
    {
#if (USE_HAL_SPI_REGISTER_CALLBACKS == 0)
      /* Init the BUS Msp */
      SPI2_MspInit();
#else
      if (IsSPI2MspCbValid == 0U)
      {
        if (SPI2_RegisterDefaultMspCallbacks() != ERROR_NONE)
        {
          ret = ERROR_MSP_FAILURE;
        }
      }
#endif /* USE_HAL_SPI_REGISTER_CALLBACKS */
      if (MX_SPI2_Init(&hbus_spi2, SPI_GetPrescaler(HAL_RCC_GetPCLK1Freq(),
                                    BUS_SPI2_BAUDRATE), SPI_DATASIZE_8BIT) != HAL_OK)
      {
        ret = ERROR_BUS_FAILURE;
      }
    }
  }
  return ret;
}

void SPI2_SetDataWidth(uint32_t w16)
{
  if (w16 != hbus_spi2.Init.DataSize)
  {
    if (w16 == SPI_DATASIZE_16BIT)
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_16BIT);
    }
    else
    {
      MX_SPI2_Init(&hbus_spi2, hbus_spi2.Init.BaudRatePrescaler, SPI_DATASIZE_8BIT);
    }
  }
}

int32_t SPI2_Send(uint8_t *pData, uint16_t Length)
{
  int32_t ret = ERROR_BUS_FAILURE;

  if (HAL_SPI_Transmit(&hbus_spi2, pData, Length, SPI2_POLL_TIMEOUT) == HAL_OK)
  {
    ret = Length;
  }

  return ret;
}

