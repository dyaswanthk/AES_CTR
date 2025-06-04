#include "uart.h"
#include "crypto.h"
#include <stdint.h>
#include<string.h>

#include <__cross_studio_io.h>

uint8_t uart1_tx[SIZE];
uint8_t uart1_rx[SIZE];
uint8_t uart1_rxFlag;

/*
UART1 gpio pins are PA9(USART_TX), PA10(USART_RX)
uart1_dma_init() function configures DMA for uart1
DMA1 Stream7 channel 4 is configured enable the data in uart1_tx[] to be transferred through uart1
DMA1 Stream2 channel 4 is configured to transfer the received data to uart1_rx[]
*/
void uart1_gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER &= ~GPIO_MODER_MODER10;
    GPIOA->MODER |= (0b10<<GPIO_MODER_MODER10_Pos);

    GPIOA->AFR[1] &= ~(0xF << ((10 - 8) * 4));      // Clear AF bits
    GPIOA->AFR[1] |=  (7 << ((10 - 8) * 4));        // Set AF7 for USART1

    GPIOA->MODER &= ~GPIO_MODER_MODER9;
    GPIOA->MODER |= (0b10<<GPIO_MODER_MODER9_Pos);

    GPIOA->AFR[1] &= ~(0xF << ((9 - 8) * 4));       // Clear AF bits
    GPIOA->AFR[1] |=  (7 << ((9 - 8) * 4));         // Set AF7 for USART1

}

void uart1_dma_init(uint8_t size)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;
    DMA2_Stream7->CR &= ~DMA_SxCR_CHSEL;            // DMA2-stream7-channel4 corresponds to USART1_TX
    DMA2_Stream7->CR |= (0b100<<DMA_SxCR_CHSEL_Pos);
    DMA2_Stream7->NDTR = size;

    DMA2_Stream7->PAR = (uint32_t)&(USART1->TDR);
    DMA2_Stream7->M0AR = (uint32_t)&(uart1_tx[0]);

    DMA2_Stream7->CR |= DMA_SxCR_MINC;
    DMA2_Stream7->CR &= ~DMA_SxCR_DIR;
    DMA2_Stream7->CR |= (0b01<<DMA_SxCR_DIR_Pos);

    DMA2_Stream2->CR &= ~DMA_SxCR_CHSEL;            // DMA2-stream2-channel4 corresponds to USART1_RX
    DMA2_Stream2->CR |= (0b100<<DMA_SxCR_CHSEL_Pos);
    DMA2_Stream2->NDTR = size;

    DMA2_Stream2->PAR = (uint32_t)&(USART1->RDR);
    DMA2_Stream2->M0AR = (uint32_t)&(uart1_rx[0]);

    DMA2_Stream2->CR |= DMA_SxCR_MINC;
    DMA2_Stream2->CR &= ~DMA_SxCR_DIR;
    DMA2_Stream2->CR |= DMA_SxCR_EN;

}




void uart1_init(uint8_t size)
{
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

// Ref. manual page no. 1041
// fPCLK = 108 MHz (APB1 clock frequency is fPCLK)
// For Baud rate of 9.6 KBps,BRR = 108MHz/9600
    USART1->BRR = 11250;

    USART1->CR1 = 0;
    USART1->CR2 = 0;
    USART1->CR3 = 0;

    USART1->CR3 |= USART_CR3_DMAR;    // DMA mode is enabled for reception
    USART1->CR3 |= USART_CR3_DMAT;    // DMA mode is enabled for transmission

    USART1->CR2 |= (0b00<<USART_CR2_STOP_Pos);    // 1 Stop bit

    USART1->CR1 &= ~USART_CR1_M1;    // 1 Start bit, 8 Data bits, n Stop bit
    USART1->CR1 &= ~USART_CR1_PCE;   // Partity bit disable

    USART1->CR1 |= USART_CR1_TE;    // Transmitter is enabled
    USART1->CR1 |= USART_CR1_RE;    // Receiver is enabled and begins searching for a start bit

    USART1->CR1 |= USART_CR1_UE;    // USART enabled

    uart1_gpio_init();

    uart1_dma_init(size);

    uart1_rx[0] = 0;
    uart1_rxFlag = 0;

}


void uart1_send(uint8_t* data, uint8_t size)
{
    if (USART1->ISR & USART_ISR_TXE)
    {
        if (DMA2->HISR & DMA_HISR_TCIF7)
        {
           
            DMA2->HIFCR |= DMA_HIFCR_CTCIF7;
        }

        DMA2_Stream7->M0AR = (uint32_t)data;
        DMA2_Stream7->NDTR = size;
        DMA2_Stream7->CR |= DMA_SxCR_EN;
    }
}




void uart1_receive(void)
{
    if (DMA2->LISR & DMA_LISR_TCIF2)  // Check if DMA transfer happened, indicating uart data reception
    {
        
        uart1_rxFlag = 1;
        
        DMA2->LIFCR |= DMA_LIFCR_CTCIF2;    // Clear the DMA flag
        DMA2_Stream2->CR |= DMA_SxCR_EN;    // Enable DMA for next data to be received
    }
 
}

void uartStateCheck()
{
 while(1){
 if(DMA2->HISR & DMA_HISR_TCIF7){
   break;
   }
 }
}