/* -------------------------------------------------------------------------
 * timer.h
 *
 * 1 ms SysTick Timer using TIM2
 *
 * Author : Nathan Heil
 * Date   : 2025-04-28
 * ------------------------------------------------------------------------- */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_

#include <stdint.h>

void TimerInit1ms(void);
uint32_t GetMsCounter(void);
void ResetMsCounter(void);

extern volatile int TLimit;

#endif /* SRC_TIMER_H_ */

