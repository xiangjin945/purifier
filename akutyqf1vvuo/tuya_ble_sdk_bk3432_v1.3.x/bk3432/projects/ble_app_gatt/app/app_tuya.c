/**
 ****************************************************************************************
 *
 * @file app_tuya.c
 *
 * @brief tuya Application
 *
 * @auth  
 *
 * @date  
 *
 *
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <string.h>
#include "app_tuya_service.h"              // tuya Application Module Definitions
#include "app_tuya.h"
#include "app.h"                    // Application Definitions
#include "app_task.h"             // application task definitions
#include "tuya_service_task.h"           // health thermometer functions
#include "co_bt.h"
#include "prf_types.h"             // Profile common types definition
#include "arch.h"                    // Platform Definitions
#include "prf.h"
#include "tuya_service.h"
#include "ke_timer.h"
#include "lld_evt.h"
#include "uart.h"
#include "tuya_ble_log.h"
#include "tuya_ble_type.h"
#include "tuya_ble_api.h"
#include "tuya_ble_main.h"
#include "app_tuya_ota.h"
#include "uart2.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

tuya_ble_device_param_t device_param = {0};


static const char auth_key_test[] = "frfds1YAltDbjyA0BmZCBfWT8OM9vDyf";
static const char device_id_test[] = "tuyac94a922f877c";
static const uint8_t mac_test[6] = {0xDC,0x23,0x4D,0x85,0xD2,0x97};


static uint8_t dp_data_array[200];
static uint16_t dp_data_len = 0;


#define APP_CUSTOM_EVENT_1  1
#define APP_CUSTOM_EVENT_2  2
#define APP_CUSTOM_EVENT_3  3
#define APP_CUSTOM_EVENT_4  4
#define APP_CUSTOM_EVENT_5  5


typedef struct {
    uint8_t data[50];
} custom_data_type_t;

void custom_data_process(int32_t evt_id,void *data)
{
    custom_data_type_t *event_1_data;
    TUYA_APP_LOG_DEBUG("custom event id = %d",evt_id);
    switch (evt_id)
    {
    case APP_CUSTOM_EVENT_1:
        event_1_data = (custom_data_type_t *)data;
        TUYA_APP_LOG_HEXDUMP_DEBUG("received APP_CUSTOM_EVENT_1 data:",event_1_data->data,50);
        break;
    case APP_CUSTOM_EVENT_2:
        break;
    case APP_CUSTOM_EVENT_3:
        break;
    case APP_CUSTOM_EVENT_4:
        break;
    case APP_CUSTOM_EVENT_5:
        break;
    default:
        break;

    }
}

custom_data_type_t custom_data;

void custom_evt_1_send_test(uint8_t data)
{
    tuya_ble_custom_evt_t event;

    for(uint8_t i=0; i<50; i++)
    {
        custom_data.data[i] = data;
    }
    event.evt_id = APP_CUSTOM_EVENT_1;
    event.custom_event_handler = (void *)custom_data_process;
    event.data = &custom_data;
    tuya_ble_custom_event_send(event);
}
#include "mcu_handler.h"
static uint16_t sn = 0;
static uint32_t time_stamp = 1587795793;
static void tuya_cb_handler(tuya_ble_cb_evt_param_t* event)
{
    UART_PRINTF("\r\event->evt = 0x%04x =======================\r\n",event->evt);
    int16_t result = 0;
    switch (event->evt)
    {
    case TUYA_BLE_CB_EVT_CONNECTE_STATUS:
        TUYA_APP_LOG_INFO("received tuya ble conncet status update event,current connect status = %d",event->connect_status);
        break;
    case TUYA_BLE_CB_EVT_DP_WRITE:
        dp_data_len = event->dp_write_data.data_len;
        memset(dp_data_array,0,sizeof(dp_data_array));
        memcpy(dp_data_array,event->dp_write_data.p_data,dp_data_len);
        TUYA_APP_LOG_HEXDUMP_DEBUG("received dp write data :",dp_data_array,dp_data_len);
        i4AppEvtMcuHandler(dp_data_array,dp_data_len);
        
        tuya_ble_dp_data_report(dp_data_array,dp_data_len);

        break;
    case TUYA_BLE_CB_EVT_DP_DATA_REPORT_RESPONSE:
        TUYA_APP_LOG_INFO("TUYA_BLE_CB_EVT_DP_DATA_REPORT_RESPONSE\r\n");
        TUYA_APP_LOG_INFO("received dp data report response result code =%d",event->dp_response_data.status);

        break;
    case TUYA_BLE_CB_EVT_DP_DATA_WTTH_TIME_REPORT_RESPONSE:
        TUYA_APP_LOG_INFO("TUYA_BLE_CB_EVT_DP_DATA_WTTH_TIME_REPORT_RESPONSE\r\n");
        TUYA_APP_LOG_INFO("received dp data report response result code =%d",event->dp_with_time_response_data.status);

        break;
    case TUYA_BLE_CB_EVT_DP_DATA_WITH_FLAG_REPORT_RESPONSE:

        break;
    case TUYA_BLE_CB_EVT_DP_DATA_WITH_FLAG_AND_TIME_REPORT_RESPONSE:

        break;
    case TUYA_BLE_CB_EVT_UNBOUND:

        TUYA_APP_LOG_INFO("received unbound req");

        break;
    case TUYA_BLE_CB_EVT_ANOMALY_UNBOUND:

        TUYA_APP_LOG_INFO("received anomaly unbound req");

        break;
    case TUYA_BLE_CB_EVT_DEVICE_RESET:

        TUYA_APP_LOG_INFO("received device reset req");

        break;
    case TUYA_BLE_CB_EVT_DP_QUERY:
        TUYA_APP_LOG_INFO("received TUYA_BLE_CB_EVT_DP_QUERY event");
        if(dp_data_len>0)
        {
            tuya_ble_dp_data_report(dp_data_array,dp_data_len);
        }
        break;
    case TUYA_BLE_CB_EVT_TIME_STAMP:
        TUYA_APP_LOG_INFO("received unix timestamp : %s ,time_zone : %d",event->timestamp_data.timestamp_string,event->timestamp_data.time_zone);
        break;
    case TUYA_BLE_CB_EVT_TIME_NORMAL:

        break;
    
    case TUYA_BLE_CB_EVT_AUTO_RECOVERY_FROM_RESETTING_TIMEOUT:
        TUYA_APP_LOG_INFO("received TUYA_BLE_CB_EVT_AUTO_RECOVERY_FROM_RESETTING_TIMEOUT event");
        break;
    case TUYA_BLE_CB_EVT_UNBIND_RESET_RESPONSE:
        if(event->reset_response_data.type == RESET_TYPE_UNBIND)
        {
            if(event->reset_response_data.status==0)
            {
                TUYA_APP_LOG_INFO("device unbind succeed.");
            }
            else
            {
                TUYA_APP_LOG_INFO("device unbind failed.");
            }
        }
        else if(event->reset_response_data.type == RESET_TYPE_FACTORY_RESET)
        {
            if(event->reset_response_data.status==0)
            {
                TUYA_APP_LOG_INFO("device factory reset succeed.");
            }
            else
            {
                TUYA_APP_LOG_INFO("device factory reset failed.");
            }
        }
    
        break;
    default:
        TUYA_APP_LOG_WARNING("app_tuya_cb_queue msg: unknown event type 0x%04x",event->evt);
        break;
    }
}

static void tuya_ble_app_init(void)
{
    device_param.device_id_len = 16;    //If use the license stored by the SDK,initialized to 0, Otherwise 16.

    if(device_param.device_id_len==16)
    {
        memcpy(device_param.auth_key,(void *)auth_key_test,AUTH_KEY_LEN);
        memcpy(device_param.device_id,(void *)device_id_test,DEVICE_ID_LEN);
        memcpy(device_param.mac_addr.addr,mac_test,6);
        device_param.mac_addr.addr_type = TUYA_BLE_ADDRESS_TYPE_RANDOM;
    }
    device_param.product_id_len = 8;
    if(device_param.product_id_len>0)
    {
        device_param.p_type = TUYA_BLE_PRODUCT_ID_TYPE_PID;
        memcpy(device_param.product_id,APP_PRODUCT_ID,8);
    }

    device_param.firmware_version = TY_APP_VER_NUM;
    device_param.hardware_version = TY_HARD_VER_NUM;

    tuya_ble_sdk_init(&device_param);
    tuya_ble_callback_queue_register(tuya_cb_handler);

    tuya_ota_init();
    TUYA_BLE_LOG_INFO("current version : %s",TY_APP_VER_STR);
    UART_PRINTF("current version : %s",TY_APP_VER_STR);
}

void app_tuya_init(void)
{
    UART_PRINTF("app_tuya_init\r\n");
    app_tuya_service_init();
    tuya_ble_app_init();

}








