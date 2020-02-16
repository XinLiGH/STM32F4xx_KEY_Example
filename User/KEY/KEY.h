/**
  ******************************************************************************
  * @file    KEY.h
  * @author  XinLi
  * @version v1.0
  * @date    24-October-2017
  * @brief   Header file for KEY.c module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>Copyright &copy; 2017 XinLi</center></h2>
  *
  * This program is free software: you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation, either version 3 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program.  If not, see <https://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

#ifndef __KEY_H
#define __KEY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Header includes -----------------------------------------------------------*/
#include "stm32f4xx.h"

/* Macro definitions ---------------------------------------------------------*/
#define KEYn                            (3)

#define KEY0_RCC_AHB1Periph_GPIO        RCC_AHB1Periph_GPIOE
#define KEY0_GPIO                       GPIOE
#define KEY0_GPIO_Pin                   GPIO_Pin_4
#define KEY0_GPIO_PuPd                  GPIO_PuPd_UP
#define KEY0_PRESS_STATUS               Bit_RESET

#define KEY1_RCC_AHB1Periph_GPIO        RCC_AHB1Periph_GPIOE
#define KEY1_GPIO                       GPIOE
#define KEY1_GPIO_Pin                   GPIO_Pin_3
#define KEY1_GPIO_PuPd                  GPIO_PuPd_UP
#define KEY1_PRESS_STATUS               Bit_RESET

#define KEY2_RCC_AHB1Periph_GPIO        RCC_AHB1Periph_GPIOE
#define KEY2_GPIO                       GPIOE
#define KEY2_GPIO_Pin                   GPIO_Pin_2
#define KEY2_GPIO_PuPd                  GPIO_PuPd_UP
#define KEY2_PRESS_STATUS               Bit_RESET

#define KEY_RCC_APB1Periph_TIM          RCC_APB1Periph_TIM2
#define KEY_TIM                         TIM2
#define KEY_TIM_Prescaler               (83)   /*!< Clock divided to 1MHz. */
#define KEY_TIM_Period                  (4999) /*!< 5ms timer interrupt. */
#define KEY_TIM_IRQn                    TIM2_IRQn
#define KEY_TIM_IRQHandler              TIM2_IRQHandler
#define KEY_TIM_IRQ_PreemptionPriority  (0)
#define KEY_TIM_IRQ_SubPriority         (0)

/* Type definitions ----------------------------------------------------------*/
typedef enum
{
  KEY_Pin0 = 0,
  KEY_Pin1 = 1,
  KEY_Pin2 = 2
}KEY_Pin;

typedef enum
{
  KEY_NoPress    = 0,
  KEY_ShortPress = 1,
  KEY_LongPress  = 2
}KEY_Status;

/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
/* Function declarations -----------------------------------------------------*/
void KEY_Init(KEY_Pin pin);
void KEY_DeInit(KEY_Pin pin);
KEY_Status KEY_GetStatus(KEY_Pin pin);
void KEY_SetPressCallback(KEY_Pin pin, void (*fun)(KEY_Status));

/* Function definitions ------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H */
