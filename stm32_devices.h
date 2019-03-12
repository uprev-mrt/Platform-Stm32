/**
  *@file Header file to include proper hal header
  *@brief SW4STM project automatically define a symbol for device type, this just uses that symbol to include the proper Hal header
  *@author Jason Berger
  *@date 03/11/2019
  */

#ifdef STM32L0xx
  #include "stm32l0xx_hal.h"
#endif

#ifdef STM32L1xx
  #include "stm32l1xx_hal.h"
#endif

#ifdef STM32L2xx
  #include "stm32l2xx_hal.h"
#endif

#ifdef STM32L3xx
  #include "stm32l3xx_hal.h"
#endif

#ifdef STM32L4xx
  #include "stm32l4xx_hal.h"
#endif

#ifdef STM32F0xx
  #include "stm32f0xx_hal.h"
#endif

#ifdef STM32F1xx
  #include "stm32f1xx_hal.h"
#endif

#ifdef STM32F2xx
  #include "stm32f2xx_hal.h"
#endif

#ifdef STM32F3xx
  #include "stm32f3xx_hal.h"
#endif

#ifdef STM32F4xx
  #include "stm32f4xx_hal.h"
#endif

#ifdef STM32F7xx
  #include "stm32f7xx_hal.h"
#endif

#ifdef STM32H7xx
  #include "stm32h7xx_hal.h"
#endif
