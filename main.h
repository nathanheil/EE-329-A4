/* -------------------------------------------------------------------------
 * main.h
 *
 * Pin Mapping for Reaction Timer
 *
 * Author : Nathan Heil
 * Date   : 2025-04-27
 * ------------------------------------------------------------------------- */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

void ErrorHandler(void);

#define B1_PIN                GPIO_PIN_13
#define B1_GPIO_PORT          GPIOC
#define LD2_PIN               GPIO_PIN_7
#define LD2_GPIO_PORT         GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
typedef enum {
   STARTUP,
   WAIT_FOR_FIRST_PRESS,
   WAIT_RANDOM_DELAY,
   WAIT_FOR_SECOND_PRESS,
   SHOW_RESULT,
   WAIT_FOR_RESET
} GameState;
extern volatile GameState current_state;
