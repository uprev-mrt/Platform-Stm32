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
mrt_status_t MRT_GATT_REGISTER_PROFILE_INIT(mrt_profile_init cbInit);
mrt_status_t MRT_GATT_REGISTER_SERVICE(mrt_gatt_svc_t* svc);
mrt_status_t MRT_GATT_REGISTER_CHAR(mrt_gatt_svc_t* svc, mrt_gatt_char_t* chr);
mrt_status_t MRT_GATT_UPDATE_CHAR(mrt_gatt_char_t* chr, uint8_t* dat, int len);


#ifdef __cplusplus
}
#endif

#endif /*STM32_GATT_MODULE_ENABLED*/

