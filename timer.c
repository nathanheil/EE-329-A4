/* -------------------------------------------------------------------------
 * timer.c
 *
 * EE 329 - 1 ms Timer using TIM2
 *
 * Author      : Nathan Heil
 * Date        : 2025-04-28
 * Description : Initializes TIM2 to generate 1 ms periodic interrupts.
 *               Increments a counter used for general-purpose timing.
 * ------------------------------------------------------------------------- */

#include "timer.h"
#include "stm32l4xx.h"
#include "main.h"
/* -------------------------------------------------------------------------
 * Global ms counter incremented by TIM2 ISR
 * ------------------------------------------------------------------------- */
static volatile uint32_t ms_counter = 0;

/* -------------------------------------------------------------------------
 * TimerInit1ms()
 * Initializes TIM2 to overflow every 1 ms and trigger an interrupt
 * ------------------------------------------------------------------------- */
void TimerInit1ms(void) {
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;              // enable TIM2 clock

   TIM2->PSC = (SystemCoreClock / 2000) - 1;           // prescaler for 2 kHz
   TIM2->ARR = 1;                                      // auto-reload = 1 count
   TIM2->CNT = 0;                                      // reset counter

   TIM2->DIER |= TIM_DIER_UIE;                         // update interrupt enable
   TIM2->SR   &= ~TIM_SR_UIF;                          // clear interrupt flag

   NVIC_SetPriority(TIM2_IRQn, 1);                     // priority = 1
   NVIC_EnableIRQ(TIM2_IRQn);                          // enable TIM2 IRQ

   TIM2->CR1 |= TIM_CR1_CEN;                           // enable counter
}

/* -------------------------------------------------------------------------
 * TIM2_IRQHandler()
 * Interrupt handler for TIM2 - fires every 1 ms
 * ------------------------------------------------------------------------- */
volatile int TLimit = 0;
void TIM2_IRQHandler(void) {
   if (TIM2->SR & TIM_SR_UIF) {                        // update interrupt flag set?
      TIM2->SR &= ~TIM_SR_UIF;                         // clear flag
      ms_counter++;                                    // increment counter
      if (ms_counter == 10000){
    	 GPIOB->ODR &= ~GPIO_PIN_7;
    	 ms_counter = 0;    	 //extern current_state;
    	 TLimit = 1;
      }
   }
}

/* -------------------------------------------------------------------------
 * GetMsCounter()
 * @returns current ms counter value
 * ------------------------------------------------------------------------- */
uint32_t GetMsCounter(void) {
   return ms_counter;
}

/* -------------------------------------------------------------------------
 * ResetMsCounter()
 * Resets ms counter to zero
 * ------------------------------------------------------------------------- */
void ResetMsCounter(void) {
   ms_counter = 0;
}
