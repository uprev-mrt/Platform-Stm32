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
#ifdef STM32WBxx_HAL_FLASH_H
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS); //If this causes compilation errors in other projects, the symbol might be specific to WB55
#endif
    
    fc->mBaseAddr = baseAddr;
    fc->mChunkSize = chunkSize;
    fc->mPageSize = pageSize;
    fc->mCursor = 0;
    return MRT_STATUS_OK;
}

mrt_status_t flash_read(flash_chunk_t* fc, uint32_t addr, uint8_t* data, uint32_t len)
{
    uint32_t* data32 = (uint32_t*) data;
    uint8_t* flashData = (__IO uint8_t*)(fc->mBaseAddr );
    // for(int i=0; i < len/4; i++ )
    // {
    //     data32[i] = *(__IO uint32_t*)(fc->mBaseAddr + addr);
    // }
    memcpy(data, flashData, len);
    
    return MRT_STATUS_OK;
}

/******************************************
 *             STM32F0xx
 * ***************************************/
#ifdef __STM32F0xx_HAL_FLASH_H
mrt_status_t flash_write(flash_chunk_t* fc, uint32_t addr, const uint8_t* data, uint32_t len)
{   

    uint32_t* cast = (uint32_t*)data;
    uint32_t comp;
    int cursor = 0;

    HAL_FLASH_Unlock();

    for(int i=0; i < len; i++)
    {
    	cursor = i/4;
        flash_read(fc, addr + i, &comp, sizeof(uint32_t));

        //Only write if it is different to save write cycles
        if(comp != cast[cursor])
        {
        	HAL_FLASH_Program(FLASH_PROC_PROGRAMWORD, fc->mBaseAddr + addr + i, cast[cursor]);
        }
        fc->mCursor = fc->mBaseAddr + addr + i +1;
    }

    HAL_FLASH_Lock();

    return MRT_STATUS_OK;
}
#endif // __STM32F0xx_HAL_FLASH_H


/******************************************
 *             STM32WBxx
 * ***************************************/
#ifdef STM32WBxx_HAL_FLASH_H
mrt_status_t flash_write(flash_chunk_t* fc, uint32_t addr, const uint8_t* data, uint32_t len)
{
    /* NOTE! writes data must be a multiple of 2 words (8 bytes) */

     __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS );

    uint64_t* cast = (uint64_t*)data;
    uint64_t comp;
    int cursor = 0;

    HAL_FLASH_Unlock();

    for(int i=0; i < len; i+=8)
    {
    	cursor = i/8;
        flash_read(fc, addr + i, &comp, sizeof(uint64_t));

        //Only write if it is different to save write cycles
        if(comp != cast[cursor])
        {
        	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, fc->mBaseAddr + addr + i, cast[cursor]);
        }
        fc->mCursor = fc->mBaseAddr + addr + i +1;
    }

    HAL_FLASH_Lock();

    return MRT_STATUS_OK;
}
#endif // STM32WBxx_HAL_FLASH_H
