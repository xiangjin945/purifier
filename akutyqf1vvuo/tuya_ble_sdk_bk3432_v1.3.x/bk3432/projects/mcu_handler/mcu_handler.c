#include <stdlib.h>    // standard lib functions
#include <stddef.h>    // standard definitions
#include <stdint.h>    // standard integer definition
#include <stdbool.h>   // boolean definition
#include "uart.h"
#include "gpio.h"
#include "timer.h"
#include "rf.h"
#include "utc_clock.h"
#include "mcu_handler.h"

void GearLedHandOff(void);
void GPIO_INIT(void)
{
    /*按键初始化*/
    gpio_config(GPIOD_3,INPUT,PULL_HIGH);
    /*led初始化*/
    gpio_config(GPIOB_1, OUTPUT, PULL_NONE);
	gpio_config(GPIOB_2, OUTPUT, PULL_NONE);//green
	gpio_config(GPIOB_3, OUTPUT, PULL_NONE); 
    gpio_config(GPIOB_4, OUTPUT, PULL_NONE);//green 
    gpio_config(GPIOD_2, OUTPUT, PULL_NONE);
    gpio_config(GPIOD_5, OUTPUT, PULL_NONE);//green
    /*电机电源初始化*/
    gpio_config(GPIOD_1, OUTPUT, PULL_NONE);
    /*负离子初始化*/
    gpio_config(GPIOD_4, OUTPUT, PULL_NONE);

    GearLedHandOff();
}

void GearLedHand1(void)
{
    gpio_set(GPIOB_1, 0);
    gpio_set(GPIOB_2, 1);

    gpio_set(GPIOB_3, 1);
    gpio_set(GPIOB_4, 0);
        
    gpio_set(GPIOD_2, 1);
    gpio_set(GPIOD_5, 0);

    gpio_set(GPIOD_1, 1);

    gpio_set(GPIOD_4, 1);
}

void GearLedHand2(void)
{
    gpio_set(GPIOB_1, 1);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 0);
    gpio_set(GPIOB_4, 1);

    gpio_set(GPIOD_2, 1);   
    gpio_set(GPIOD_5, 0);
    
    gpio_set(GPIOD_1, 1);
}

void GearLedHand3(void)
{
    gpio_set(GPIOB_1, 1);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 1);
    gpio_set(GPIOB_4, 0);

    gpio_set(GPIOD_2, 0);    
    gpio_set(GPIOD_5, 1);
    
    gpio_set(GPIOD_1, 1);
}

void GearLedHandOff(void)
{
    gpio_set(GPIOB_1, 0);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 0);
    gpio_set(GPIOB_4, 0);
        
    gpio_set(GPIOD_5, 0);
    gpio_set(GPIOD_2, 0);

    gpio_set(GPIOD_1, 0);

    gpio_set(GPIOD_4, 0);
}


bool gPowerOn = false,ButtonStu = false;
uint8_t CurrentGear = 0;
void i4KeyPowerLongPressCallBack()
{
    switch(CurrentGear){
        case 1:
            UART_PRINTF("power on\n");
            UART_PRINTF("Grar:1\n");
            GearLedHand1();
            break;
        case 2:
            UART_PRINTF("Grar:2\n");
            GearLedHand2();
            break;
        case 3:
            UART_PRINTF("Grar:3\n");
            GearLedHand3();
            break;
        case 4:
            UART_PRINTF("power off!\n");
            GearLedHandOff();
            CurrentGear = 0;
            break;
        default:
            break;
    }
}

static uint8_t i4CheckPressPowerKey(void)
{
    static uint32_t value = 0;
    uint8_t res = 0;
    if(!gpio_get_input(GPIOD_3)){
        if(!ButtonStu){
            value = i4utc_get_time();
    
        }
        ButtonStu = true;
        
    }
    if(gpio_get_input(GPIOD_3)&&ButtonStu){  
        value = i4utc_get_time() - value;
        UART_PRINTF("sokai %d\n", value);
        if(value >= 5){
            UART_PRINTF("BLE Start\n");
        }else{
            CurrentGear++;
        }
        ButtonStu = false;
        res = 1;
    }
    return res;
}

void i4PowerButtonProcessing(void)
{
    uint8_t powerKeyVal = 0;
    powerKeyVal = i4CheckPressPowerKey();
    if(powerKeyVal == 1){
        i4KeyPowerLongPressCallBack();
    }

}

void i4AppEvtMcuHandler(uint8_t *data_buf,uint16_t data_len)
{
    if(data_len!=4){
        UART_PRINTF("len>4!\n");
    }
    switch(data_buf[0])
    {
        case DP_SWITCH_MODE:
            if(data_buf[3]){
                GearLedHand1();
            }else{
                GearLedHandOff();
            }
            break;
        case DP_WIND_SPEED:
            if(data_buf[3]==0){
                
            }
            if(data_buf[3]==1){
                GearLedHand1();
            }
            if(data_buf[3]==2){
                GearLedHand2();
            }
            if(data_buf[3]==3){
                GearLedHand3();
            }
            break;
        default:
            break;
    }
}
