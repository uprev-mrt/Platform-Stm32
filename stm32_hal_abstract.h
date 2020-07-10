
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

/* BLE Gatt abstraction */
#ifdef STM32_GATT_MODULE_ENABLED
  #include "Platforms/STM32/aci_ble/stm32_gatt_abstract.h"
#endif


//Delay Abstraction
#define MRT_DELAY_MS(ms) HAL_Delay(ms)

//Uart Abstraction
typedef HAL_StatusTypeDef mrt_uart_status_t;
#ifdef HAL_UART_MODULE_ENABLED
  typedef UART_HandleTypeDef* mrt_uart_handle_t;

  #define MRT_UART_TX(handle, data, len, timeout) HAL_UART_Transmit(handle, data, len, timeout)
  #define MRT_UART_RX(handle, data, len, timeout) mrt_stm32_uart_read(handle, data, len, timeout)

  /**
   *  The STM32 HAL does not provide a non-blocking uart receive
   *  The normal receive will return if it was successful or failed,
   *  This will return the number of byte received before the timeout 
   */
  int mrt_stm32_uart_read(mrt_uart_handle_t handle, uint8_t* data, int len, int timeout);
#else
  typedef uint32_t* mrt_uart_handle_t;
#endif


//GPIO Abstraction
typedef struct{
  GPIO_TypeDef* port;
  uint16_t pin;
} mrt_gpio_t;

typedef GPIO_TypeDef* mrt_gpio_port_t;
#define NULL_GPIO_Port NULL
#define NULL_Pin 0
#define MRT_GPIO_MODE_INPUT GPIO_MODE_INPUT
#define MRT_GPIO_MODE_OUTPUT GPIO_MODE_OUTPUT_PP

#define MRT_GPIO(pin) (mrt_gpio_t){pin##_GPIO_Port, pin##_Pin }		//used to create a mrt_gpio_t from the stm32_hal symbol for the pin
#define MRT_GPIO_WRITE( gpio , val ) HAL_GPIO_WritePin(gpio.port, gpio.pin, val)
#define MRT_GPIO_READ( gpio ) HAL_GPIO_ReadPin(gpio.port, gpio.pin)
#define MRT_GPIO_PORT_WRITE(port, mask, val) (port->ODR = (port->ODR & ~mask) | (mask & val))   /* unverified */ 
#define MRT_GPIO_PORT_READ(port) (port->IDR)                                                    /* unverified */ 
#define MRT_GPIO_PORT_SET_DIR(port,mask, mode) mrt_stm32_gpio_port_set_dir(port,mask,mode);
uint32_t mrt_stm32_gpio_port_set_dir(GPIO_TypeDef* port, uint32_t mask, uint32_t mode);

//I2C Abstraction
typedef HAL_StatusTypeDef mrt_i2c_status_t;
#ifdef HAL_I2C_MODULE_ENABLED
  typedef I2C_HandleTypeDef* mrt_i2c_handle_t;
  #define MRT_I2C_MASTER_TRANSMIT(handle ,addr,data,len, stop, timeout) HAL_I2C_Master_Transmit(handle , addr, data, len, timeout)
  #define MRT_I2C_MASTER_RECEIVE(handle ,addr, data, len, stop, timeout) HAL_I2C_Master_Receive(handle , addr, data, len, timeout)
  #define MRT_I2C_MEM_WRITE(handle, addr, mem_addr, mem_size, data, len, timeout ) HAL_I2C_Mem_Write(handle ,addr, mem_addr, mem_size, data, len, timeout)
  #define MRT_I2C_MEM_READ(handle, addr, mem_addr, mem_size, data, len, timeout ) HAL_I2C_Mem_Read(handle ,addr, mem_addr, mem_size, data, len, timeout)
#else
  typedef uint32_t* mrt_i2c_handle_t;
#endif

//SPI Abstraction
typedef HAL_StatusTypeDef mrt_spi_status_t;
#ifdef HAL_SPI_MODULE_ENABLED
  typedef SPI_HandleTypeDef* mrt_spi_handle_t;
  #define MRT_SPI_TRANSFER(handle ,tx, rx ,len, timeout) HAL_SPI_TransmitReceive(handle, tx, rx, len, timeout)
  #define MRT_SPI_TRANSMIT(handle, tx, len, timeout) HAL_SPI_Transmit(handle, tx, len, timeout)
  #define MRT_SPI_RECIEVE(handle, tx, len, timeout) HAL_SPI_Transmit(handle, tx, len, timeout)
#else
  typedef uint32_t* mrt_spi_handle_t;
  #define MRT_SPI_TRANSFER(handle ,tx, rx ,len, timeout) 0
  #define MRT_SPI_TRANSMIT(handle, tx, len, timeout) 0
  #define MRT_SPI_RECIEVE(handle, tx, len, timeout) 0
#endif



//printf
#define MRT_PRINTF(f_, ...) printf((f_), ##__VA_ARGS__)

#ifdef __cplusplus
 }
#endif
