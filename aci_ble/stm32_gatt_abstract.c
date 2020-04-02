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
#include "Platforms/STM32/aci_ble/stm32_gatt_abstract.h"
#include "common_blesvc.h"
#include "Utilities/Interfaces/GattServer/gatt_server.h"

/* Private Variables ---------------------------------------------------------*/
static bool ServerInitialized = false;                                      /* flag to make sure server is initialized before updating chars*/
mrt_profile_init f_profileInit;                                             /* function pointer to inject profile init function into framework*/
static attrib_table_entry_t attribute_table[MRT_GATT_MAX_ATTRIBUTE_COUNT];  /* Attibute entry table*/

/* Functions -----------------------------------------------------------------*/

static void clear_attribute_table()
{
    for(int i=0; i < MRT_GATT_MAX_ATTRIBUTE_COUNT; i++)
    {
        attribute_table[i].mType = ATTR_NONE;
        attribute_table[i].mPtr = NULL;
    }
}

static SVCCTL_EvtAckStatus_t gatt_event_handler(void *Event)
{
    /* From ST example*/
	SVCCTL_EvtAckStatus_t return_value;
    hci_event_pckt *event_pckt;
    evt_blue_aci *blue_evt;
    aci_gatt_attribute_modified_event_rp0    * attribute_modified;

    return_value = SVCCTL_EvtNotAck;
    event_pckt = (hci_event_pckt *)(((hci_uart_pckt*)Event)->data);
    
    /* Variables for converting ST event structure to MRT*/
    attrib_table_entry_t attr_entry;
    mrt_gatt_evt_t mrt_evt;
    mrt_evt.mType =GATT_EVT_NONE;
    mrt_evt.mChar = NULL;

    switch(event_pckt->evt)
    {
        case EVT_VENDOR: /* Handle vendor/profile specific events, i.e. ignore low level events*/
        {
        	blue_evt = (evt_blue_aci*)event_pckt->data;
            switch(blue_evt->ecode)
            {       
                    case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:
                    {
                        attribute_modified = (aci_gatt_attribute_modified_event_rp0*)blue_evt->data;
                        
                        /* look up handle*/
                        attr_entry = attribute_table[attribute_modified->Attr_Handle];
                        
                        /* build mrt_gatt_evt_t from st event struct*/
                        mrt_evt.mData.data = attribute_modified->Attr_Data;
                        mrt_evt.mData.len = attribute_modified->Attr_Data_Length;
                        switch(attr_entry.mType)
                        {
                            case ATTR_CHR_VALUE:
                                mrt_evt.mType = GATT_EVT_VALUE_WRITE; 
                                mrt_evt.mChar = (mrt_gatt_char_t*) attr_entry.mPtr;
                                break;
                            case ATTR_CHR_DESCRIPTOR:
                                mrt_evt.mType = GATT_EVT_DESCR_WRITE; 
                                mrt_evt.mChar = (mrt_gatt_char_t*) attr_entry.mPtr;
                                break;
                            default:
                            	break;
                        }
                    
                    }
            }
        }
    }

    /* If an event was built, send it the handler for the appropriate characteristic*/
    if(mrt_evt.mChar != NULL)
    {
        mrt_evt.mChar->cbEvent(&mrt_evt);
    }

    return return_value;
}

/**
 * @brief Initializes the services. This is declared weakly in ST's framework, so we re-implement to inject out initializer
 * 
 */
void SVCCTL_SvcInit(void)
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



    /*	Register the event handler to the BLE controller */
    SVCCTL_RegisterSvcHandler(gatt_event_handler);

    ServerInitialized = true;     /* Mark server as intialized */
    f_profileInit();              /* This is where we set our profile init function*/


	  return;
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
	return MRT_STATUS_OK;
}


uint32_t MRT_GATT_REGISTER_SERVICE(mrt_gatt_svc_t* svc)
{
    Char_UUID_t  st_uuid16; /*uses ST uuid struct*/
    uint8_t st_uuid_type;

    /* before registering the first service, clear the table*/
    MRT_ON_FIRST
    {
    	/* clear attribute table*/
        clear_attribute_table();
    }

    /* get attribute count */
    uint8_t attrib_count =1;                    /* one attribute for the service itself*/
    for(int i = 0; i < svc->mCharCount; i++)
    {
        attrib_count+= 2;                       /* 2 attributes for each characteristic */
        if(svc->mChars[i]->mProps & MRT_GATT_PROP_NOTIFY)
        	attrib_count++;						/* 1 extra attribute if char has notifications */
    }


    /*Add Service to ACI*/
    convert_uuid(&svc->mUuid, &st_uuid16,  &st_uuid_type);
    aci_gatt_add_service(st_uuid_type,
                      (Service_UUID_t *) &st_uuid16,
                      PRIMARY_SERVICE,
                      attrib_count, /*Max_Attribute_Records*/
                      &svc->mHandle);
    
    /* add attrib handle to table*/
    attribute_table[svc->mHandle].mType = ATTR_SERVICE;
    attribute_table[svc->mHandle].mPtr = (void*) svc;

    /* Add each Characteristic in the service*/
    for(int i = 0; i < svc->mCharCount; i++)
    {
        mrt_gatt_char_t* chr = svc->mChars[i];

        convert_uuid(&chr->mUuid, &st_uuid16,  &st_uuid_type);      /*convert uuid*/
        aci_gatt_add_char(svc->mHandle,
                      st_uuid_type, &st_uuid16,
                      chr->mSize,
                      chr->mProps | CHAR_PROP_WRITE ,                                 /* Gatt server uses the same prop flags, no need to convert for STM32*/
                      ATTR_PERMISSION_NONE,
					  GATT_NOTIFY_ATTRIBUTE_WRITE, /* gattEvtMask */
                      10, /* encryKeySize */
                      1, /* isVariable */
                      &chr->mHandle);
            /* add char attrib handle to table*/
            attribute_table[chr->mHandle].mType = ATTR_CHR;
            attribute_table[chr->mHandle].mPtr = (void*) chr;

            /* add value attrib handle to table*/
            attribute_table[chr->mHandle+1].mType = ATTR_CHR_VALUE;
            attribute_table[chr->mHandle+1].mPtr = (void*) chr;

             /* add Descriptor attrib handle to table*/
            attribute_table[chr->mHandle+2].mType = ATTR_CHR_DESCRIPTOR;
            attribute_table[chr->mHandle+2].mPtr = (void*) chr;

    }
   return MRT_STATUS_OK;
}

mrt_status_t MRT_GATT_UPDATE_CHAR(mrt_gatt_char_t* chr, uint8_t* data, int len)
{
    mrt_status_t status = MRT_STATUS_ERROR;

    if(!ServerInitialized) /* If server is not initialized, return error*/
    {
        return status;
    }
        

    status = aci_gatt_update_char_value(chr->mSvc->mHandle ,
                             chr->mHandle,
                              0, /* charValOffset */
                             len, /* charValueLen */
                             data);
    
    return status;
}



#endif /*STM32_GATT_MODULE_ENABLED*/


