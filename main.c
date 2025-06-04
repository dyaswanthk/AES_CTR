#include "stm32f756xx.h"
#include "main.h"
#include "uart.h"
#include "sys_clk.h"
#include "crypto.h"
#include "timer.h"
#include <string.h>
#include <cross_studio_io.h>

volatile uint8_t to_encrypt;
uint8_t text[SIZE] = "The brown fox jumps over the lazy dog" ;
uint8_t tx_size;


int main()
  {
  tx_size = strlen(text);
  if (tx_size%16) tx_size = ((tx_size/16)+1)*16;//rounding of to lowest multiple of 16
  SysClock_init();
  uart1_init(tx_size);
  MX_CRYP_Init();
  timer2_init();

  while(1) 
  {
    if (to_encrypt)
    {
      AESEncrypt((uint32_t*) text);
      memcpy(uart1_tx,ciphertext,tx_size);
      uart1_send(uart1_tx,tx_size);
      uartStateCheck();
      to_encrypt = 0;
    }
    uart1_receive();
    if(uart1_rxFlag)
    {
       AESDecrypt();
       uart1_rxFlag = 0;
     
    }
  
  }
} 


void TIM2_IRQHandler(void)
{
  if(TIM2->SR & TIM_SR_UIF)
  {
    TIM2->SR &= ~TIM_SR_UIF;
  }
  to_encrypt = 1;
}