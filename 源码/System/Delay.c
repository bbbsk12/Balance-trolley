#include "stm32f10x.h"
#define HAL_MAX_DELAY      0xFFFFFFFFU
typedef enum
{
  HAL_TICK_FREQ_10HZ         = 100U,
  HAL_TICK_FREQ_100HZ        = 10U,
  HAL_TICK_FREQ_1KHZ         = 1U,
  HAL_TICK_FREQ_DEFAULT      = HAL_TICK_FREQ_1KHZ
} HAL_TickFreqTypeDef;



HAL_TickFreqTypeDef uwTickFreq = HAL_TICK_FREQ_DEFAULT;  /* 1KHz */

__IO uint32_t uwTick;

__weak uint32_t HAL_GetTick(void)
{
  return uwTick;
}

__weak void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
}


__weak void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

   while ((HAL_GetTick() - tickstart) < wait)
  {
  }
}
void SysTick_Handler(void)
{

	
  HAL_IncTick();

}
