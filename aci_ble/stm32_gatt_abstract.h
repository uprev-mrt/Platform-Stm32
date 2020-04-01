/**
 * @file gatt_server.h
 * @author Jason Berger
 * @brief Abstract/Generic Gatt server.
 * @version 0.1
 * @date 2020-02-21
 *
 *
 */
#ifdef STM32_GATT_MODULE_ENABLED

#pragma once
#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>


/* Exported types ------------------------------------------------------------*/
typedef uint32_t mrt_gatt_evt_status_t;
typedef uint16_t mrt_gatt_chr_handle_t;
typedef uint16_t mrt_gatt_svc_handle_t;
typedef uint32_t mrt_status_t;

typedef struct mrt_gatt_svc_t mrt_gatt_svc_t;                             //forward declare for self referencing callback
typedef struct mrt_gatt_char_t mrt_gatt_char_t;                             //forward declare for self referencing callback
typedef mrt_status_t (*mrt_profile_init)(void);

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

/**
 * @brief registers a callback to our profile init function. This way we can inject it into their init function
 * @param cbInit callback to initialize profile
 * @return mrt_status_t 
 */
mrt_status_t MRT_GATT_REGISTER_PROFILE_INIT(mrt_profile_init cbInit);

/**
 * @brief Register our service with the server
 * @param svc ptr to service (with all characteristics already added)
 */
mrt_status_t MRT_GATT_REGISTER_SERVICE(mrt_gatt_svc_t* svc);

/**
 * @brief update characteristic value
 * @param chr ptr to characteristic
 * @param data ptr to data to update from
 * @param len size of data (in bytes)
 */
mrt_status_t MRT_GATT_UPDATE_CHAR(mrt_gatt_char_t* chr, uint8_t* data, int len);


#ifdef __cplusplus
}
#endif

#endif /*STM32_GATT_MODULE_ENABLED*/

