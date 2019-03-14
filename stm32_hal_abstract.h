
/**
  *@file stm32_hal_abstract.h
  *@brief Abstraction header for STM32 MCUs using the HAL framework
  *@author Jason Berger
  *@date 4/6/2018
  */

#pragma once

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include "main.h"


#ifdef __cplusplus
 }
 namespace Platforms
 {
#endif

//Delay Abstraction
#define MRT_DELAY_MS(ms) HAL_Delay(ms)

//Uart Abstraction
#ifdef HAL_UART_MODULE_ENABLED
  typedef UART_HandleTypeDef* mrt_uart_handle_t;
  typedef HAL_StatusTypeDef mrt_uart_status_t;
  #define MRT_UART_TX(handle, data, len, timeout) HAL_UART_Transmit(handle, data, len, timeout)
  #define MRT_UART_RX(handle, data, len, timeout) HAL_UART_Retrieve(handle, data, len, timeout)
#endif


//GPIO Abstraction
typedef struct{
  GPIO_TypeDef* port;
	uint16_t pin;
} mrt_gpio_t;
#define MRT_GPIO_WRITE(pin,val) HAL_GPIO_WritePin(pin.port, pin.pin, val)
#define MRT_GPIO_READ(pin) HAL_GPIO_ReadPin(pin.port, pin.pin)

//I2C Abstraction
#ifdef HAL_I2C_MODULE_ENABLED
  typedef I2C_HandleTypeDef* mrt_i2c_handle_t;
  typedef HAL_StatusTypeDef mrt_i2c_status_t;
  #define MRT_I2C_MASTER_TRANSMIT(handle ,addr,data,len, stop, timeout) HAL_I2C_Master_Transmit(handle , addr, data, len, timeout)
  #define MRT_I2C_MASTER_RECEIVE(handle ,addr, data, len, stop, timeout) HAL_I2C_Master_Receive(handle , addr, data, len, timeout)
  #define MRT_I2C_MEM_WRITE(handle, addr, mem_addr, mem_size, data, len, timeout ) HAL_I2C_Mem_Write(handle ,addr, mem_addr, mem_size, data, len, timeout)
  #define MRT_I2C_MEM_READ(handle, addr, mem_addr, mem_size, data, len, timeout ) HAL_I2C_Mem_Read(handle ,addr, mem_addr, mem_size, data, len, timeout)
#endif

//SPI Abstraction
#ifdef HAL_SPI_MODULE_ENABLED
  typedef SPI_HandleTypeDef* mrt_spi_handle_t;
  typedef HAL_StatusTypeDef mrt_spi_status_t;
  #define MRT_SPI_TRANSFER(handle ,tx, rx ,len, timeout) HAL_SPI_TransmitReceive(handle, tx, rx, len, timeout)
  #define MRT_SPI_TRANSMIT(handle, tx, len, timeout) HAL_SPI_Transmit(handle, tx, len, timeout)
  #define MRT_SPI_RECIEVE(handle, tx, len, timeout) HAL_SPI_Transmit(handle, tx, len, timeout)
#endif

//printf
#define MRT_PRINTF(f_, ...) printf((f_), __VA_ARGS__)

#ifdef __cplusplus
 }
#endif
