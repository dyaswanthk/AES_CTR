#include "main.h"
#include "uart.h"
#include "crypto.h"
#include <string.h>
#include <cross_studio_io.h>


CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyCRYP[8] __ALIGN_END = {
		    0x603deb10, 0x15ca71be,
    0x2b73aef0, 0x857d7781,
    0x1f352c07, 0x3b6108d7,
    0x2d9810a3, 0x0914dff4};
__ALIGN_BEGIN static const uint32_t pInitVectCRYP[4] __ALIGN_END = {
		0x00010203, 0x04050607, 0x08090a0b, 0x0c0d0e0f};


DMA_HandleTypeDef hdma_cryp_in;
DMA_HandleTypeDef hdma_cryp_out;

__ALIGN_BEGIN uint8_t ciphertext[SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t decryptedtext[SIZE] __ALIGN_END;
__ALIGN_BEGIN uint8_t encryptedtext[SIZE]__ALIGN_END;

uint8_t count = 0;


 void MX_CRYP_Init(void)
{

  hcryp.Instance = CRYP;
  hcryp.Init.DataType = CRYP_DATATYPE_8B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_256B;
  hcryp.Init.pKey = (uint32_t *)pKeyCRYP;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectCRYP;
  hcryp.Init.Algorithm = CRYP_AES_CTR;
  hcryp.Init.DataWidthUnit = CRYP_DATAWIDTHUNIT_BYTE;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  MX_DMA_Init(); 
}

 void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);
  /* DMA2_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);

}


void AESEncrypt(uint32_t* plaintext)
{
  HAL_StatusTypeDef status = HAL_CRYP_Encrypt_DMA(&hcryp, plaintext,tx_size, (uint32_t*)ciphertext);
  {
   if(status != HAL_OK)
  {
    Error_Handler();
  }
  }
}

void AESDecrypt(void)
{
   if( HAL_CRYP_Decrypt_DMA(&hcryp, (uint32_t*)uart1_rx,tx_size, (uint32_t*)decryptedtext)!= HAL_OK)
   { 

        Error_Handler();
     }

debug_printf("%d %s\n",count,decryptedtext);
count++;
}



void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}