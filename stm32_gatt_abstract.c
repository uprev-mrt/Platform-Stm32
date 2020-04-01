/**
 * @file gatt_server.c
 * @author Jason Berger
 * @brief Abstract/Generic Gatt server.
 * @version 0.1
 * @date 2020-02-21
 *
 *
 */
#ifdef STM32_GATT_MODULE_ENABLED


/* Includes ------------------------------------------------------------------*/
#include "Platforms/STM32/stm32_gatt_abstract.h"
#include "common_blesvc.h"
#include "Devices/RF/BLE/GattServer/gatt_server.h"

mrt_profile_init f_profileInit;

SVCCTL_SvcInit(void)
{
	  BLS_Init();
	  CRS_STM_Init();
	  DIS_Init();
	  EDS_STM_Init();
	  HIDS_Init();
	  HRS_Init();
	  HTS_Init();
	  IAS_Init();
	  LLS_Init();
	  TPS_Init();
	  MOTENV_STM_Init();
	  OTAS_STM_Init();
	  BVOPUS_STM_Init();
	  MESH_Init();


	  f_profileInit();


	  return;
}

static SVCCTL_EvtAckStatus_t Seal_Event_Handler(void *Event)
{
	return SVCCTL_EvtNotAck;
}


/**
 * @brief converts from MRT uuid structure to ST
 */
void convert_uuid(mrt_gatt_uuid_t* src, Char_UUID_t* dst, uint8_t* st_type)
{
    if(src->mType == e16Bit)
    {
        dst->Char_UUID_16 = src->m16Bit;
        *st_type = UUID_TYPE_16;
    }
    else
    {
        *st_type = UUID_TYPE_128;
        memcpy((uint8_t*)dst->Char_UUID_128, (uint8_t*)src->m128Bit, 16);
    }
}


mrt_status_t MRT_GATT_REGISTER_PROFILE_INIT(mrt_profile_init cbInit)
{
	f_profileInit = cbInit;
}

uint32_t MRT_GATT_REGISTER_SERVICE(mrt_gatt_svc_t* svc)
{
    Char_UUID_t  st_uuid16; /*uses ST uuid struct*/
    uint8_t st_uuid_type;
    uint8_t st_props;

    /* get attribute count */
    uint8_t attrib_count =1;                    /* one attribute for the service itself*/
    for(int i = 0; i < svc->mCharCount; i++)
    {
        attrib_count+= 2;                       /* 2 attributes for each characteristic */
        if(svc->mChars[i].mProps & MRT_GATT_PROP_NOTIFY)
        	attrib_count++;						/* 1 extra attribute if char has notifications */
    }

    /**
      *	Register the event handler to the BLE controller
      */
    SVCCTL_RegisterSvcHandler(Seal_Event_Handler);

    /*Add Service to ACI*/
    convert_uuid(&svc->mUuid, &st_uuid16,  &st_uuid_type);
    aci_gatt_add_service(st_uuid_type,
                      (Service_UUID_t *) &st_uuid16,
                      PRIMARY_SERVICE,
                      attrib_count, /*Max_Attribute_Records*/
                      &svc->mHandle);

    /* Add each Characteristic in the service*/
    for(int i = 0; i < svc->mCharCount; i++)
    {
        mrt_gatt_char_t* chr = &svc->mChars[i];

        convert_uuid(&chr->mUuid, &st_uuid16,  &st_uuid_type);      /*convert uuid*/
        aci_gatt_add_char(svc->mHandle,
                      st_uuid_type, &st_uuid16,
                      chr->mSize,
                      chr->mProps ,                                 /* Gatt server uses the same prop flags, no need to convert for STM32*/
                      ATTR_PERMISSION_NONE,
                      GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable */
                      &chr->mHandle);

    }
}



#endif /*STM32_GATT_MODULE_ENABLED*/


