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
#include "app_tuya.h"
#include "app.h"
#include "BK3432_reg.h"
#include "pwm.h"
#include "tuya_ble_api.h"
#include "stdbool.h"
#include "flash.h"

#define PWM_VALUE 65
uint8_t BleConnectStatus = 0;
uint8_t CurrentGear = 0;
uint8_t xSumLife =  180;//总寿命180天 
uint8_t xLifeCount = 1;
bool gPowerOn = false;
bool xDataFirstReport = true;
bool ButtonStu = false;
bool xLifeday = false;




void GearLedHandOff(void);
void i4Dp05_ReportHandler(void);
void i4Dp16_ReportHandler(void);

xData xdata;
xTime time;
void i4PwmInit(uint8_t channel,uint16_t value,uint16_t duty)
{
	PWM_DRV_DESC drv_desc;
	drv_desc.channel = channel; 
	drv_desc.mode = 0x01;
	drv_desc.pre_divid = 0;
	drv_desc.end_value = value;
	drv_desc.duty_cycle = duty;
	drv_desc.p_Int_Handler = NULL;

	REG_AHB0_ICU_PWMCLKCON |= (1<<1);
    REG_AHB0_ICU_PWMCLKCON &= ~(7<<12);
    REG_AHB0_ICU_PWMCLKCON |= (8<<12);

	pwm_init(&drv_desc);

}

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
    /*电机电源和pwm脚初始化*/
    gpio_config(GPIOD_1, OUTPUT, PULL_NONE);
    gpio_config(GPIOB_0, OUTPUT, PULL_NONE);
    /*负离子初始化*/
    gpio_config(GPIOD_4, OUTPUT, PULL_NONE);


    GearLedHandOff();
}

void GearLedHand1(void)
{
    CurrentGear = 1;
    gpio_set(GPIOB_1, 0);
    gpio_set(GPIOB_2, 1);

    gpio_set(GPIOB_3, 1);
    gpio_set(GPIOB_4, 0);
        
    gpio_set(GPIOD_2, 1);
    gpio_set(GPIOD_5, 0);

    gpio_set(GPIOD_1, 1);
    gpio_set(GPIOB_0, 1);//pwm脚使能

    i4PwmInit(0,PWM_VALUE,PWM_VALUE*47/100);

    gpio_set(GPIOD_4, 1);
    gPowerOn = true;
}

void GearLedHand2(void)
{
    CurrentGear = 2;
    gpio_set(GPIOB_1, 1);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 0);
    gpio_set(GPIOB_4, 1);

    gpio_set(GPIOD_2, 1);   
    gpio_set(GPIOD_5, 0);
    
    gpio_set(GPIOD_1, 1);
    gpio_set(GPIOB_0, 1);//pwm脚使能
    i4PwmInit(0,PWM_VALUE,PWM_VALUE*64/100);
}

void GearLedHand3(void)
{
    CurrentGear = 3;
    gpio_set(GPIOB_1, 1);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 1);
    gpio_set(GPIOB_4, 0);

    gpio_set(GPIOD_2, 0);    
    gpio_set(GPIOD_5, 1);
    
    gpio_set(GPIOD_1, 1);
    gpio_set(GPIOB_0, 1);//pwm脚使能
    i4PwmInit(0,PWM_VALUE,PWM_VALUE*85/100);
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
    gpio_set(GPIOB_0, 0);//pwm

    gpio_set(GPIOD_4, 0);
    gPowerOn = false;
}

void i4SleepLedHandler(void)
{
    gpio_set(GPIOB_1, 0);
    gpio_set(GPIOB_2, 0);

    gpio_set(GPIOB_3, 0);
    gpio_set(GPIOB_4, 0);
        
    gpio_set(GPIOD_5, 0);
    gpio_set(GPIOD_2, 0);
}

/*************************Tuya DP report**********************************/
uint8_t dp_buf[4];
void DP_BOOL_REPORT(uint8_t dp,uint8_t data)
{
    memset(dp_buf, 0, sizeof(dp_buf));
    dp_buf[0] = dp;
    dp_buf[1] = 0x01;//BOOL
    dp_buf[2] = 0x01;
    dp_buf[3] = data;
    tuya_ble_dp_data_report(dp_buf,sizeof(dp_buf));
}
void DP_VALUE_REPORT(uint8_t dp,uint8_t data)
{   
    memset(dp_buf, 0, sizeof(dp_buf));
    dp_buf[0] = dp;
    dp_buf[1] = 0x02;//value
    dp_buf[2] = 0x01;
    dp_buf[3] = data;
    tuya_ble_dp_data_report(dp_buf,sizeof(dp_buf));
}
void DP_ENUM_REPORT(uint8_t dp,uint8_t data)
{
    memset(dp_buf, 0, sizeof(dp_buf));
    dp_buf[0] = dp;
    dp_buf[1] = 0x04;//enum
    dp_buf[2] = 0x01;
    dp_buf[3] = data;
    tuya_ble_dp_data_report(dp_buf,sizeof(dp_buf));
}

/*************************Tuya DP report end******************************/

/*************************Cartridge life***********************************/
void i4LifeCalculation(void)
{
    UART_PRINTF("sumlife:%d\n",xSumLife);
    xSumLife -= xLifeCount;
    if(xSumLife>=0){
        DP_VALUE_REPORT(DP_CARTRIDGE_LIFE,xSumLife);
    }
    
}

void i4read_falsh_save_time(uint32_t apptime)
{   
    uint8_t buf[4];
    flash_read(FLASH_SPACE_TYPE_MAIN, START_TIME_SAVE_ADDRESS/4, 4,buf);
    
    time.start = (buf[3] | (buf[2] << 8) | (buf[1] << 16) | (buf[0] << 24));
    UART_PRINTF("*start time:%#x*****read:%#x,%#x,%#x,%#x\n",time.start,buf[0],buf[1],buf[2],buf[3]);
    if(buf[0] == 0xff)
    {
        time.start = apptime;
        UART_PRINTF("first time start :%x\n", time.start);
        buf[3] = time.start;
        buf[2] = time.start >> 8;
        buf[1] = time.start >> 16;
        buf[0] = time.start >> 24;
        flash_write(FLASH_SPACE_TYPE_MAIN, START_TIME_SAVE_ADDRESS/4, 4,buf);
        // flash_read(FLASH_SPACE_TYPE_MAIN, START_TIME_SAVE_ADDRESS/4, 4, buf);
        // UART_PRINTF("*start time:%#x*****read:%#x,%#x,%#x,%#x\n",time.start,buf[0],buf[1],buf[2],buf[3]);
        DP_VALUE_REPORT(DP_CARTRIDGE_LIFE,xSumLife);
    }
    else
    {
        UART_PRINTF("apptime:%d,time.start:%d,%d\n",apptime,time.start,xLifeCount);
        if((apptime - time.start) > SAVE_DAY_TIME_SEC*xLifeCount)
        {
            xLifeday = true;
            xLifeCount++;
        }
    }

    if(xLifeday)i4LifeCalculation();
    
    
}

/*************************Cartridge life end******************************/




void i4KeyPowerLongPressCallBack()
{
    //1.上报当前为手动模式
    xdata.mode = 0;
    DP_ENUM_REPORT(DP_SELECT_MODE,xdata.mode);
    xdata.speed = CurrentGear;
    //2.依据按键对应档位交互
    switch(CurrentGear){
        case 1:   
            UART_PRINTF("power on\n");
            UART_PRINTF("Grar:1\n");
            GearLedHand1();
            UART_PRINTF("shou dong mode!\n");
            DP_ENUM_REPORT(DP_WIND_SPEED,xdata.speed);
            DP_BOOL_REPORT(DP_SWITCH_MODE,(uint8_t)gPowerOn);
            break;
        case 2:
            UART_PRINTF("Grar:2\n");
            GearLedHand2();
            DP_ENUM_REPORT(DP_WIND_SPEED,xdata.speed);
            break;
        case 3:
            UART_PRINTF("Grar:3\n");
            GearLedHand3();
            DP_ENUM_REPORT(DP_WIND_SPEED,xdata.speed);
            break;
        case 4:
            UART_PRINTF("power off!\n");
            GearLedHandOff();
            CurrentGear = 0;
            DP_BOOL_REPORT(DP_SWITCH_MODE,CurrentGear);
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
            // tuya_ble_device_unbind();//TODO:长按五秒后解绑，暂时先注释。
            // appm_disconnect();
            // appm_start_advertising();
        }else{
            CurrentGear++;
        }
        ButtonStu = false;
        res = 1;
    }
    return res;
}



/**
 * @description: APP指令下发mcu端对应DP点处理
 * @param {uint8_t} *data_buf
 * @param {uint16_t} data_len
 * @return: {*}
 */
void i4AppEvtMcuHandler(uint8_t *data_buf,uint16_t data_len)
{
    uint8_t dp_data = data_buf[3];
    if(data_len!=4){
        UART_PRINTF("len>4!\n");
    }
    switch(data_buf[0])
    {
        case DP_SWITCH_MODE:
            if(dp_data){
                GearLedHand1();
                
            }else{
                GearLedHandOff();
            }
            break;
        case DP_SELECT_MODE:
            xdata.mode = dp_data;
            if(dp_data==0){

            }
            if(dp_data==1){

            }
            if(dp_data==2){
                GearLedHand2();
            }
            if(dp_data==3){
               GearLedHand1();
               i4SleepLedHandler();
            }
            break;
        case DP_WIND_SPEED:
            xdata.speed = dp_data;
            if(dp_data==0){
                
            }
            if(dp_data==1){
                GearLedHand1();
            }
            if(dp_data==2){
                GearLedHand2();
            }
            if(dp_data==3){
                GearLedHand3();
            }
            break;
        case DP_LIGHTING:
            xdata.light = dp_data;
            if(dp_data){
                i4KeyPowerLongPressCallBack();
            }else{
                i4SleepLedHandler();
            }
            break;
        case DP_CARTRIDGE_RESET:
              if(dp_data){
                flash_erase(FLASH_SPACE_TYPE_MAIN,START_TIME_SAVE_ADDRESS/4,4);
                xSumLife = 180;
                DP_VALUE_REPORT(DP_CARTRIDGE_LIFE,xSumLife);

            }else{
                
            }  
            break;
        default:
            break;
    }
}

void Dp_data_init(void)
{
    xdata.mode = 0;
    xdata.speed = 0;
    xdata.light = 1;
}

void i4PowerButtonProcessing(void)
{
    uint8_t powerKeyVal = 0;
    powerKeyVal = i4CheckPressPowerKey();
    if(powerKeyVal == 1){
        i4KeyPowerLongPressCallBack();
    }
    //app绑定时初始化面板值
    if(BleConnectStatus==4 && xDataFirstReport){
        UART_PRINTF("==4====BleConnectStatus:%d,%d,%d\n",(uint8_t)gPowerOn,xdata.mode,xdata.speed);
        DP_BOOL_REPORT(DP_SWITCH_MODE,(uint8_t)gPowerOn);
        DP_ENUM_REPORT(DP_SELECT_MODE,xdata.mode);
        DP_ENUM_REPORT(DP_WIND_SPEED,xdata.speed);
        DP_BOOL_REPORT(DP_LIGHTING,xdata.light);
        xDataFirstReport = false;
    }
    //app掉线重连时更新面板值
    if(BleConnectStatus==3 && xDataFirstReport){
        UART_PRINTF("==3====BleConnectStatus:%d,%d,%d\n",(uint8_t)gPowerOn,xdata.mode,xdata.speed);
        DP_BOOL_REPORT(DP_SWITCH_MODE,(uint8_t)gPowerOn);
        DP_ENUM_REPORT(DP_SELECT_MODE,xdata.mode);
        DP_ENUM_REPORT(DP_WIND_SPEED,xdata.speed);
        DP_BOOL_REPORT(DP_LIGHTING,xdata.light);

        xDataFirstReport = false;
    }

}
