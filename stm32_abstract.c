/**
  *@file stm32_hal_abstract.c
  *@brief Abstraction header for STM32 MCUs using the HAL framework
  *@author Jason Berger
  *@date 4/6/2018
  */

 #include "stm32_hal_abstract.h"


 int mrt_stm32_uart_read(mrt_uart_handle_t handle, uint8_t* data, int len, int timeout)
 {
    /* attempt a uart read*/
    if(HAL_UART_Receive(handle, data, len, timeout) == HAL_TIMEOUT  )
    {
        len -= handle->RxXferCount; /* when the read is started, RxXferCount gets set to len, and decrements for each byte received*/
    }

    return len;
 }
