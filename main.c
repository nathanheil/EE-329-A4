/* -------------------------------------------------------------------------
 * main.c
 *
 * EE 329 A4 - Reaction Timer Game
 *
 * Author      : Nathan Heil
 * Version     : 1.0
 * Date        : 2025-04-27
 * Description : Reaction game using a pushbutton and LED. The game waits for
 *               a user press, then randomly delays before lighting the LED.
 *               The user’s reaction time to turn it off is measured and shown
 *               on a 2x16 LCD. System resets after 10 seconds or a second press.
 * Target Board: STM32L4A6ZG Nucleo-144
 * Peripherals : LED on PB7, Button on PC13, LCD display (user-connected)
 * ------------------------------------------------------------------------- */

#include "main.h"
#include "stm32l4xx.h"
#include "delay.h"
#include "lcd.h"
#include "timer.h"
#include <stdio.h>

/* -------------------------------------------------------------------------
 * Enum to define the states of the reaction timer game
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */
 volatile GameState current_state;
static volatile uint32_t random_delay = 0;     // ms delay before LED turns on
static volatile uint32_t led_on_time = 0;      // reaction time in ms

/* -------------------------------------------------------------------------
 * SetupGPIO()
 * Configures GPIOB pin 7 as output for LED
 * Configures GPIOC pin 13 as input for pushbutton (no pull)
 * ------------------------------------------------------------------------- */
static void SetupGPIO(void) {
   RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN | RCC_AHB2ENR_GPIOBEN;

   // PC13 = Input (Button)
   GPIOC->MODER &= ~(3 << (2 * 13));
   GPIOC->PUPDR &= ~(3 << (2 * 13));
   GPIOC->MODER &= ~(3 << (2 * 0));
    GPIOC->PUPDR &= ~(3 << (2 * 0));

   // PB7 = Output Push-Pull, High Speed (LED)
   GPIOB->MODER &= ~(3 << (2 * 7));
   GPIOB->MODER |= (1 << (2 * 7));
   GPIOB->OTYPER &= ~(1 << 7);
   GPIOB->OSPEEDR |= (3 << (2 * 7));
   GPIOB->PUPDR &= ~(3 << (2 * 7));
}

/* -------------------------------------------------------------------------
 * ButtonIsPressed()
 * Returns 1 if PC13 is LOW (button pressed), 0 otherwise
 * ------------------------------------------------------------------------- */
static inline int ButtonIsPressed(void) {
   return ((GPIOC->IDR & GPIO_PIN_13) == 0);
}

/* -------------------------------------------------------------------------
 * LCDShowPrompt()
 * Displays prompt to user to begin the reaction game
 * ------------------------------------------------------------------------- */
static void LCDShowPrompt(void) {
   LCD_Clear();
   LCD_SetCursor(0, 0);
   LCD_WriteString("A4 Reaction game");
   LCD_SetCursor(1, 0);
   LCD_WriteString("push sw to begin");
}

/* -------------------------------------------------------------------------
 * LCDShowResult()
 * Displays reaction time in format X.XXX sec
 * @param time_ms - reaction time in milliseconds
 * ------------------------------------------------------------------------- */
static void LCDShowResult(uint32_t time_ms) {
   char buf[20];
   int s = time_ms / 1000;
   int m = time_ms % 1000;

   LCD_Clear();
   LCD_SetCursor(0, 0);
   LCD_WriteString("LED ON TIME:");
   sprintf(buf, "%d.%03d sec", s, m);
   LCD_SetCursor(1, 0);
   LCD_WriteString(buf);
}

/* -------------------------------------------------------------------------
 * main()
 * Implements state machine for reaction timer
 * ------------------------------------------------------------------------- */
int main(void) {
   SysTick_Init();        // Initialize microsecond delay system
   TimerInit1ms();        // Timer for millisecond counters
   SetupGPIO();           // Configure button and LED pins
   LCD_Init();            // Initialize LCD display
   LCDShowPrompt();       // Initial screen

   while (1) {
      switch (current_state) {

         case STARTUP:
            LCDShowPrompt();
            current_state = WAIT_FOR_FIRST_PRESS;
            break;

         case WAIT_FOR_FIRST_PRESS:
            if (ButtonIsPressed()) {
               while (ButtonIsPressed());         // Wait for release
               DelayUs(20000);                    // Debounce delay (20 ms)
               random_delay = 1000 + (GetMsCounter() * 37 % 3000);  // 1–4 sec
               ResetMsCounter();                  // Start timer for random delay
               current_state = WAIT_RANDOM_DELAY;
            }
            break;

         case WAIT_RANDOM_DELAY:
            if (GetMsCounter() >= random_delay) {
               GPIOB->ODR |= GPIO_PIN_7;          // Turn ON LED
               ResetMsCounter();                  // Start reaction timer
               while (ButtonIsPressed());         // Ensure button is released
               DelayUs(20000);                    // Debounce delay
               current_state = WAIT_FOR_SECOND_PRESS;
            }
            break;

         case WAIT_FOR_SECOND_PRESS:
            if (TLimit == 1) {
               GPIOB->ODR &= ~GPIO_PIN_7;     // Make sure LED is off
               TLimit = 0;
               ResetMsCounter();
               current_state = STARTUP;
            } else if (ButtonIsPressed()) {
               led_on_time = GetMsCounter();
               GPIOB->ODR &= ~GPIO_PIN_7;
               while (ButtonIsPressed());     // Wait for release
               DelayUs(20000);                // Debounce
               current_state = SHOW_RESULT;
            }
            break;
         case SHOW_RESULT:
            LCDShowResult(led_on_time);
            ResetMsCounter();
            current_state = WAIT_FOR_RESET;
            break;

         case WAIT_FOR_RESET:
            if (ButtonIsPressed()) {
               while (ButtonIsPressed());         // Debounce
               current_state = STARTUP;
            }
            break;
      }
   }
}

/* -------------------------------------------------------------------------
 * ErrorHandler()
 * Disables interrupts and enters infinite loop
 * ------------------------------------------------------------------------- */
void ErrorHandler(void) {
   __disable_irq();
   while (1) {}
}

/* -------------------------------------------------------------------------
 * assert_failed()
 * Stub function for assert macros (if enabled)
 * ------------------------------------------------------------------------- */
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
   // User can add logging here if desired
}
#endif
