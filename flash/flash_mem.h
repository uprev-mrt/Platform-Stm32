/**
 * @file flash_memory.h
 * @author Jason Berger
 * @brief Library for accessing flash data
 * @date 2020-06-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Platforms/Common/mrt_platform.h"

#ifdef __STM32F0xx_HAL_FLASH_H

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct{
    uint32_t mBaseAddr;  //Start address for this chunk  
    uint32_t mChunkSize; //Size of this chunk in bytes
    uint32_t mPageSize;  //Page size of memory
    uint32_t mCursor;    //cursor for next write/read
}flash_chunk_t;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @brief initialize flash memory chunk
 * @param fc ptr to flash chunk struct
 * @param baseAddr base address of chunk
 * @param chunkSize size of chunk
 * @param pageSize page size for physical memory
 * @return mrt_status_t 
 */
mrt_status_t flash_init(flash_chunk_t* fc, uint32_t baseAddr, uint32_t chunkSize, uint32_t pageSize );

/**
 * @brief Writes data to flash memory chunk
 * @param fc ptr to flash chunk struct
 * @param addr address offset from chunks base Address to write to (in bytes ) 
 * @param len number of bytes to write
 * @param data ptr to data to write
 * @return mrt_status_t 
 */
mrt_status_t flash_write(flash_chunk_t* fc, uint32_t addr, const uint8_t* data, uint32_t len);

/**
 * @brief Reads data from flash memory chunk
 * @param fc ptr to flash chunk struct
 * @param addr address offset from chunks base Address to read from (in bytes ) 
 * @param len number of bytes to read
 * @param data ptr to store data
 * @return mrt_status_t 
 */
mrt_status_t flash_read(flash_chunk_t* fc, uint32_t addr, uint8_t* data, uint32_t len);

#endif // __STM32F0xx_HAL_FLASH_H