/**
  *@file stm32_hal_abstract.c
  *@brief Abstraction header for STM32 MCUs using the HAL framework
  *@author Jason Berger
  *@date 4/6/2018
  */

 #include "stm32_hal_abstract.h"

#ifdef HAL_UART_MODULE_ENABLED
/**
 * @brief Non-blocking uart receive function for stm32
 * @param handle handle to uart periph 
 * @param data ptr to store received bytes
 * @param len number of bytes ro receive
 * @param timeout timeout in ms to wait for data
 * @return number of bytes read. 
 */
int mrt_stm32_uart_read(UART_HandleTypeDef* handle, uint8_t* data, int len, int timeout)
{
  HAL_StatusTypeDef status;
  /* attempt a uart read, if read times out, check how many bytes were received */
  status = HAL_UART_Receive(handle, data, len, timeout);

  if(status == HAL_TIMEOUT)
  {
      len -= handle->RxXferCount; /* when the read is started, RxXferCount gets set to len, and decrements for each byte received*/
  }
  else if(status != HAL_OK) /* If response is not TIMEOUT or OK, something went wrong */
  {
    len = 0;
  }

  return len; /* return number of bytes read */
}


#endif

#ifdef HAL_TIM_MODULE_ENABLED

/**
 * @brief initializes mrt struct for PWM
 * @param pwm - ptr to mrt_pwm_t
 * @param tim - ptr to STM32 Timer 
 * @param channel - ptr to OC channel config
 * @param config - ptr to OC channel config
 * @return uin32_t 
 */
uint32_t mrt_stm32_pwm_init(mrt_pwm_t* pwm, TIM_HandleTypeDef* tim, uint32_t channel,  TIM_OC_InitTypeDef* config)
{
  pwm->mTimer = tim;
  pwm->mChannel = channel;
  memcpy( (void*) &pwm->mConfig, (void*) config, sizeof(TIM_OC_InitTypeDef));
  pwm->mFlags = PWM_FLAG_NONE;

  return 0;
}

uint32_t mrt_stm32_pwm_set_duty(mrt_pwm_t* pwm, float val)
{
  uint32_t pulse = (val * pwm->mTimer->Init.Period) / 100; //convert percentage to pulse

  return mrt_stm32_pwm_set_pulse(pwm, pulse);
}

uint32_t mrt_stm32_pwm_set_pulse(mrt_pwm_t* pwm, uint32_t val)
{
  HAL_StatusTypeDef status;

  pwm->mConfig.Pulse = val;


  if(pwm->mFlags & PWM_FLAG_COMP) /* set Complementary channel signal*/
  {
   
    status = HAL_TIMEx_PWMN_Stop(pwm->mTimer, pwm->mChannel);
    if (HAL_TIM_PWM_ConfigChannel(pwm->mTimer, &pwm->mConfig, pwm->mChannel) != HAL_OK) 
    { 
        Error_Handler(); 
    } 

    status = HAL_TIMEx_PWMN_Start(pwm->mTimer, pwm->mChannel);
  }
  else  /* set  channel signal*/
  {

    status = HAL_TIM_PWM_Stop(pwm->mTimer,  pwm->mChannel);

    if (HAL_TIM_PWM_ConfigChannel(pwm->mTimer, &pwm->mConfig, pwm->mChannel) != HAL_OK) 
    { 
        Error_Handler(); 
    } 

    status = HAL_TIM_PWM_Start(pwm->mTimer, pwm->mChannel);
  }

  return status;
}

#endif

uint32_t mrt_stm32_gpio_port_set_dir(GPIO_TypeDef* port, uint32_t mask, uint32_t mode)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = mask;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(port, &GPIO_InitStruct);
}
