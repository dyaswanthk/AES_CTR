#ifndef CRYPTO_H
#define CRYPTO_H

#include "stm32f7xx_hal.h"

#define SIZE 255

extern DMA_HandleTypeDef hdma_cryp_in;
extern DMA_HandleTypeDef hdma_cryp_out;


__ALIGN_BEGIN extern uint8_t ciphertext[SIZE] __ALIGN_END;
__ALIGN_BEGIN extern uint8_t encryptedtext[SIZE]__ALIGN_END;


void MX_CRYP_Init(void);
void MX_DMA_Init(void);
void AESEncrypt(uint32_t* plaintext_buffer);
void AESDecrypt();
void CheckCRYPStatus(void);
void Error_Handler(void);


#endif