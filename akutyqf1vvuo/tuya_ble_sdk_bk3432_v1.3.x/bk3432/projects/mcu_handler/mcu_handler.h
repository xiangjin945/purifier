#ifndef _MCU_HANDLER_
#define _MCU_HANDLER_

#define DP_SWITCH_MODE  1
#define DP_SELECT_MODE  3
#define DP_WIND_SPEED   4


void GPIO_INIT(void);
void i4PowerButtonProcessing(void);
void i4AppEvtMcuHandler(uint8_t *data_buf,uint16_t data_len);
#endif