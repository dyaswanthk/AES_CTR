#ifndef UART1_H
#define UART1_H

#include "stm32f756xx.h"
#include "crypto.h"

//#define UART_BUFFER_TX1_SIZE 16
//#define UART_BUFFER_RX1_SIZE 16

extern uint8_t uart1_tx[SIZE];
extern uint8_t uart1_rx[SIZE];
extern uint8_t uart1_rxFlag;

void uart1_gpio_init(void);
void uart1_dma_init(uint8_t size);
void uart1_init(uint8_t size);
void uart1_send(uint8_t* data, uint8_t size);
void uart1_receive(void);
void uartStateCheck();


#endif