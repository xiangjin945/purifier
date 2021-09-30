#ifndef _MCU_HANDLER_
#define _MCU_HANDLER_

#include <stdint.h>
#include "stdbool.h"

#define DP_SWITCH_MODE       (1)
#define DP_SELECT_MODE       (3)
#define DP_WIND_SPEED        (4)
#define DP_LIGHTING          (8)
#define DP_CARTRIDGE_RESET   (11)
#define DP_CARTRIDGE_LIFE    (16)
#define DP_COUNT_DOWN        (18)

#define SAVE_DAY_TIME_SEC                (86400)//一天的时间
#define START_TIME_SAVE_ADDRESS		     (0x1B800)

typedef struct XData{
    uint8_t mode;
    uint8_t speed;
    uint8_t light;
}xData;

typedef struct XTime{
    uint32_t start;
    uint32_t next;
}xTime;


extern uint8_t BleConnectStatus;
extern bool xDataFirstReport;

void GPIO_INIT(void);
void i4PowerButtonProcessing(void);
void i4AppEvtMcuHandler(uint8_t *data_buf,uint16_t data_len);
void Dp_data_init(void);
void i4read_falsh_save_time(uint32_t apptime);
#endif          