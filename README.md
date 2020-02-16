# STM32F4xx_KEY_Example

使用STM32F4的定时器TIM，来实现独立按键的扫描。适用于STM32F4全部芯片。

## 开发环境

* 固件库：STM32F4xx_DSP_StdPeriph_Lib_V1.8.0
* 编译器：ARMCC V5.06
* IDE：Keil uVision5
* 操作系统：Windows 10 专业版

## API

* void KEY_Init(KEY_Pin pin)
* void KEY_DeInit(KEY_Pin pin)
* KEY_Status KEY_GetStatus(KEY_Pin pin)
* void KEY_SetPressCallback(KEY_Pin pin, void (*fun)(KEY_Status))

## 注意

修改接口需要注意，按键的检测电平、使用的TIM和增减按键的个数。对应修改KEY_PRESS_STATUS宏、TIM时钟配置函数和源文件的变量定义。
