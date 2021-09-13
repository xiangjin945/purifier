# USER GUIDE



## Flash layout

![](https://images.tuyacn.com/fe-static/docs/img/8368e547-5c53-4752-967a-cb42c2d1e6ac.jpg)

## 说明

- 说明文档：

​       [tuya ble sdk](https://docs.tuya.com/zh/iot/device-development/access-mode-link/ble-chip-sdk/tuya-ble-sdk-user-guide?id=K9h5zc4e5djd9)

​       [tuya ble sdk demo](https://docs.tuya.com/zh/iot/device-development/access-mode-link/ble-chip-sdk/tuya-ble-sdk-demo-instruction-manual?id=K9gq09szmvy2o)

- `tuya_ble_sdk_Demo_Project_bk3432 `是基于精简版 tuya ble sdk 开发，所以 sdk 说明文档中有的功能和接口在此 sdk 中不支持。
- 从 flash layout 可以看出应用代码空间只有 45K，在开启 log 的情况下编译后会超出45K大小，但是只要不测试 OTA，固件总大小超过45k是没有问题的，需要测试 OTA 的时候把 log 关掉即可，关闭 log 的方式是将`tuya_ble_config.h`中的 `TUYA_BLE_LOG_ENABLE` 和 `TUYA_APP_LOG_ENABLE` 配置为0 ，Release版固件大小不得超过44.6K 。
- 不支持通过串口协议下载固件，开发调试时每次必须通过 BekenHIDTool 软件使用 spi 下载器 下载固件，所以每次下载固件时都会清空整个flash信息，包括授权信息等。
- UART2用于授权以及和外部MCU通信( 波特率9600 )，UART1用于log输出( 波特率921600 )。
- 工程路径：`tuya_ble_sdk_Demo_Project_bk3432\bk3432\projects\ble_app_gatt`
- output目录下的`BinConvert_3432.exe.1`重命名为`BinConvert_3432.exe` 。
- 基于版本号v1.2.x 的 sdk 开发的固件不能通过 OTA 的方式升级到 基于v1.3.x 版本 sdk 开发的固件，sdk 版本号见`tuya_ble_sdk_Demo_Project_bk3432\bk3432\projects\ble_app_gatt\app\tuya_ble_sdk\tuya_ble_sdk_version.h`  。



