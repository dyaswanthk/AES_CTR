/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f7xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "crypto.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */
extern DMA_HandleTypeDef hdma_cryp_in;

extern DMA_HandleTypeDef hdma_cryp_out;

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */

  /* USER CODE END MspInit 1 */
}

/**
  * @brief CRYP MSP Initialization
  * This function configures the hardware resources used in this example
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==CRYP)
  {
    /* USER CODE BEGIN CRYP_MspInit 0 */

    /* USER CODE END CRYP_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CRYP_CLK_ENABLE();

    /* CRYP DMA Init */
    /* CRYP_IN Init */
    hdma_cryp_in.Instance = DMA2_Stream6;
    hdma_cryp_in.Init.Channel = DMA_CHANNEL_2;
    hdma_cryp_in.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_cryp_in.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cryp_in.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cryp_in.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_cryp_in.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_cryp_in.Init.Mode = DMA_NORMAL;
    hdma_cryp_in.Init.Priority = DMA_PRIORITY_LOW;
    hdma_cryp_in.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_cryp_in) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcryp,hdmain,hdma_cryp_in);

    /* CRYP_OUT Init */
    hdma_cryp_out.Instance = DMA2_Stream5;
    hdma_cryp_out.Init.Channel = DMA_CHANNEL_2;
    hdma_cryp_out.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_cryp_out.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_cryp_out.Init.MemInc = DMA_MINC_ENABLE;
    hdma_cryp_out.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_cryp_out.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_cryp_out.Init.Mode = DMA_NORMAL;
    hdma_cryp_out.Init.Priority = DMA_PRIORITY_LOW;
    hdma_cryp_out.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_cryp_out) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hcryp,hdmaout,hdma_cryp_out);

    /* CRYP interrupt Init */
    HAL_NVIC_SetPriority(CRYP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CRYP_IRQn);
    /* USER CODE BEGIN CRYP_MspInit 1 */

    /* USER CODE END CRYP_MspInit 1 */

  }

}

/**
  * @brief CRYP MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param hcryp: CRYP handle pointer
  * @retval None
  */
void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* hcryp)
{
  if(hcryp->Instance==CRYP)
  {
    /* USER CODE BEGIN CRYP_MspDeInit 0 */

    /* USER CODE END CRYP_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CRYP_CLK_DISABLE();

    /* CRYP DMA DeInit */
    HAL_DMA_DeInit(hcryp->hdmain);
    HAL_DMA_DeInit(hcryp->hdmaout);

    /* CRYP interrupt DeInit */
    HAL_NVIC_DisableIRQ(CRYP_IRQn);
    /* USER CODE BEGIN CRYP_MspDeInit 1 */

    /* USER CODE END CRYP_MspDeInit 1 */
  }

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
