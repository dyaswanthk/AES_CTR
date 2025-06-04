
/*  Configures system clock to 216MHz using HSE as PLL source
 HSE (8MHz) -> PLL (PLLM=4, PLLN=216, PLLP=2) -> SYSCLK (216MHz)
  AHB = 216MHz, APB1 = 54MHz, APB2 = 108MHz
 */

#include "sys_clk.h"


void SysClock_init(void)
{
    // Enable PWR clock
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    
    // Configure voltage regulator to scale 1 (high performance)
    PWR->CR1 &= ~PWR_CR1_VOS;
    PWR->CR1 |= PWR_CR1_VOS_0 | PWR_CR1_VOS_1; // VOS = 11 (Scale 1)
    
    // Wait for voltage regulator to be ready
  //  while (!(PWR->CSR1 & PWR_CSR1_VOSRDY));
    
    // Configure Flash latency for 216MHz operation
    // FLASH_LATENCY_7 = 7 wait states for 216MHz at 3.3V
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_7WS;
    
    // Enable instruction cache and prefetch buffer
   // FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_PRFTEN;
    
    // Configure HSE oscillator (8MHz external crystal)
    RCC->CR |= RCC_CR_HSEON;
    
    // Wait for HSE to be ready
    while (!(RCC->CR & RCC_CR_HSERDY));
    
    // Configure PLL
    // Turn off PLL first
    RCC->CR &= ~RCC_CR_PLLON;
    
    // Wait for PLL to be turned off
    while (RCC->CR & RCC_CR_PLLRDY);
    
    // Configure PLL parameters
    RCC->PLLCFGR = 0; // Clear all bits first
    
    // PLL Source = HSE (bit 22 = 1)
    RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
    
    // PLLM = 4 (divide HSE by 4: 8MHz/4 = 2MHz)
    RCC->PLLCFGR |= (4 << RCC_PLLCFGR_PLLM_Pos);
    
    // PLLN = 216 (multiply by 216: 2MHz * 216 = 432MHz)
    RCC->PLLCFGR |= (216 << RCC_PLLCFGR_PLLN_Pos);
    
    // PLLP = 2 (divide by 2: 432MHz/2 = 216MHz)
    RCC->PLLCFGR |= (0 << RCC_PLLCFGR_PLLP_Pos); // 00 = divide by 2
    
    // PLLQ = 2 (for USB, SDMMC, RNG: 432MHz/2 = 216MHz)
    RCC->PLLCFGR |= (2 << RCC_PLLCFGR_PLLQ_Pos);
    
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    
    // Wait for PLL to be ready
    while (!(RCC->CR & RCC_CR_PLLRDY));
    
    // Enable Over-Drive mode for 216MHz operation
    PWR->CR1 |= PWR_CR1_ODEN;
    
    // Wait for Over-Drive to be ready
    while (!(PWR->CSR1 & PWR_CSR1_ODRDY));
    
    // Enable Over-Drive switching
    PWR->CR1 |= PWR_CR1_ODSWEN;
    
    // Wait for Over-Drive switching to be ready
    while (!(PWR->CSR1 & PWR_CSR1_ODSWRDY));
    
    // Configure system clocks
    // AHB prescaler = 1 (SYSCLK not divided)
    RCC->CFGR &= ~RCC_CFGR_HPRE;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
    
    // APB1 prescaler = 4 (HCLK/4 = 216/4 = 54MHz - must be <= 54MHz)
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
    
    // APB2 prescaler = 2 (HCLK/2 = 216/2 = 108MHz - must be <= 108MHz)
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    // Select PLL as system clock source
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    
    // Wait for PLL to be used as system clock source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
    
    // Update SystemCoreClock variable (if using CMSIS)
    // SystemCoreClock = 216000000;
}
