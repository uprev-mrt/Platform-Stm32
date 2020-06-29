/**
 * @file flash_memory.c
 * @author Jason Berger
 * @brief Library for accessing flash data
 * @date 2020-06-23
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once 

/* Includes ------------------------------------------------------------------*/
#include "flash_mem.h"

/* Private Variables ---------------------------------------------------------*/
/* Functions -----------------------------------------------------------------*/

mrt_status_t flash_init(flash_chunk_t* fc, uint32_t baseAddr, uint32_t chunkSize, uint32_t pageSize )
{
    fc->mBaseAddr = baseAddr;
    fc->mChunkSize = chunkSize;
    fc->mPageSize = pageSize;
    fc->mCursor = 0;
    return MRT_STATUS_OK;
}

mrt_status_t flash_write(flash_chunk_t* fc, uint32_t addr, const uint8_t* data, uint32_t len)
{
    uint32_t* cast = (uint16_t*)data;
    uint32_t comp;

    HAL_FLASH_Unlock();

    for(int i=0; i < len; i+=4)
    {
        flash_read(fc, fc->mBaseAddr + addr + i, &comp, 4);

        //Only write if it is different to save write cycles
        if(comp != cast[i])
        {
            HAL_FLASH_Program(FLASH_PROC_PROGRAMWORD, fc->mBaseAddr + addr + i, cast[i]);
        }
        fc->mCursor = fc->mBaseAddr + addr + i +1;
    }

    HAL_FLASH_Lock();

    return MRT_STATUS_OK;
}

mrt_status_t flash_read(flash_chunk_t* fc, uint32_t addr, uint8_t* data, uint32_t len)
{
    uint8_t* flashData = (*(__IO uint8_t*)(fc->mBaseAddr + addr));
    memcpy(data, flashData, len);
    
    return MRT_STATUS_OK;
}
