/**
 * \file tuya_ble_app_uart_common_handler.h
 *
 * \brief 
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk 
 */

#ifndef _TUYA_BLE_APP_UART_COMMON_HANDLER_H_
#define _TUYA_BLE_APP_UART_COMMON_HANDLER_H_

#include <stdint.h>
#include "tuya_ble_internal_config.h"

#ifdef __cplusplus
extern "C" {
#endif


#define TUYA_BLE_UART_COMMON_HEART_MSG_TYPE       	        0x00
#define TUYA_BLE_UART_COMMON_SEARCH_PID_TYPE      	        0x01
#define TUYA_BLE_UART_COMMON_CK_MCU_TYPE          	        0x02
#define TUYA_BLE_UART_COMMON_REPORT_WORK_STATE_TYPE  	    0x03
#define TUYA_BLE_UART_COMMON_RESET_TYPE             	    0x04
#define TUYA_BLE_UART_COMMON_SEND_CMD_TYPE           	    0x06
#define TUYA_BLE_UART_COMMON_SEND_STATUS_TYPE        	    0x07
#define TUYA_BLE_UART_COMMON_QUERY_STATUS            	    0x08
#define TUYA_BLE_UART_COMMON_MODULE_UNBOUND           	    0x09
#define TUYA_BLE_UART_COMMON_RF_TEST               	        0x0E
#define TUYA_BLE_UART_COMMON_SEND_STORAGE_TYPE              0xE0
#define TUYA_BLE_UART_COMMON_SEND_TIME_SYNC_TYPE            0xE1
#define TUYA_BLE_UART_COMMON_MODIFY_ADV_INTERVAL		    0xE2
#define TUYA_BLE_UART_COMMON_TURNOFF_SYSTEM_TIME		    0xE4
#define TUYA_BLE_UART_COMMON_ENANBLE_LOWER_POWER		    0xE5
#define TUYA_BLE_UART_COMMON_SEND_ONE_TIME_PASSWORD_TOKEN   0xE6
#define TUYA_BLE_UART_COMMON_ACTIVE_DISCONNECT			    0xE7

#define TUYA_BLE_UART_COMMON_QUERY_MCU_VERSION			    0xE8
#define TUYA_BLE_UART_COMMON_MCU_SEND_VERSION			    0xE9

//#define TUYA_BLE_UART_COMMON_MCU_OTA_REQUEST			    0xEA
//#define TUYA_BLE_UART_COMMON_MCU_OTA_FILE_INFO			    0xEB
//#define TUYA_BLE_UART_COMMON_MCU_OTA_FILE_OFFSET	        0xEC
//#define TUYA_BLE_UART_COMMON_MCU_OTA_DATA 			        0xED
//#define TUYA_BLE_UART_COMMON_MCU_OTA_END			        0xEE

//#define TUYA_BLE_UART_COMMON_MODIFY_BLE_CONN_INTERVER       
#define TUYA_BLE_UART_COMMON_BLE_OTA_STATUS            	    0xF0


/**
 * @brief   Function for transmit ble data from peer devices to tuya sdk.
 *
 * @note    This function must be called from where the ble data is received. 
 *.
 * */

void tuya_ble_uart_common_process(uint8_t *p_in_data,uint16_t in_len);


#ifdef __cplusplus
}
#endif

#endif // _TUYA_BLE_APP_UART_COMMON_HANDLER_H_

