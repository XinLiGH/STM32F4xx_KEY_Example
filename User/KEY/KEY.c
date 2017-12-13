/**
  ******************************************************************************
  * @file    KEY.c
  * @author  XinLi
  * @version v1.0
  * @date    24-October-2017
  * @brief   KEY module driver.
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

/* Header includes -----------------------------------------------------------*/
#include "KEY.h"
#include <string.h>

/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
static      GPIO_TypeDef *KEY_GPIO[KEYn]                     = {KEY1_GPIO, KEY2_GPIO, KEY3_GPIO};
static      uint16_t      KEY_GPIO_Pin[KEYn]                 = {KEY1_GPIO_Pin, KEY2_GPIO_Pin, KEY3_GPIO_Pin};
static      uint32_t      KEY_RCC_AHB1Periph_GPIO[KEYn]      = {KEY1_RCC_AHB1Periph_GPIO, KEY2_RCC_AHB1Periph_GPIO, KEY3_RCC_AHB1Periph_GPIO};
static __IO int           keyInputHighCount[KEYn]            = {0};
static __IO int           keyInputLowCount[KEYn]             = {0};
static __IO KEY_Status    keyStatus[KEYn]                    = {KEY_NoPress, KEY_NoPress, KEY_NoPress};
static __IO KEY_Status    keyGetStatus[KEYn]                 = {KEY_NoPress, KEY_NoPress, KEY_NoPress};
static __IO void        (*keyShortPressCallback[KEYn])(void) = {NULL};
static __IO void        (*keyLongPressCallback[KEYn])(void)  = {NULL};

/* Function declarations -----------------------------------------------------*/
/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Key initializes.
  * @param  None.
  * @return None.
  */
void KEY_Init(void)
{
  for(int i = 0; i < KEYn; i++)
  {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    RCC_AHB1PeriphClockCmd(KEY_RCC_AHB1Periph_GPIO[i], ENABLE);
    
    GPIO_InitStructure.GPIO_Pin   = KEY_GPIO_Pin[i];
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(KEY_GPIO[i], &GPIO_InitStructure);
    
    keyInputHighCount[i]     = 0;
    keyInputLowCount[i]      = 0;
    keyStatus[i]             = KEY_NoPress;
    keyGetStatus[i]          = KEY_NoPress;
    keyShortPressCallback[i] = NULL;
    keyLongPressCallback[i]  = NULL;
  }
  
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure = {0};
  
  RCC_APB1PeriphClockCmd(KEY_RCC_APB1Periph_TIM, ENABLE);
  
  TIM_TimeBaseInitStructure.TIM_Prescaler     = KEY_TIM_Prescaler;
  TIM_TimeBaseInitStructure.TIM_CounterMode   = TIM_CounterMode_Up;
  TIM_TimeBaseInitStructure.TIM_Period        = KEY_TIM_Period;
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(KEY_TIM, &TIM_TimeBaseInitStructure);
  
  TIM_ClearFlag(KEY_TIM, TIM_FLAG_Update);
  TIM_ITConfig(KEY_TIM, TIM_IT_Update, ENABLE);
  TIM_Cmd(KEY_TIM, ENABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure = {0};
  
  NVIC_InitStructure.NVIC_IRQChannel                   = KEY_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = KEY_TIM_IRQ_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = KEY_TIM_IRQ_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Key de-initializes.
  * @param  None.
  * @return None.
  */
void KEY_DeInit(void)
{
  for(int i = 0; i < KEYn; i++)
  {
    keyInputHighCount[i]     = 0;
    keyInputLowCount[i]      = 0;
    keyStatus[i]             = KEY_NoPress;
    keyGetStatus[i]          = KEY_NoPress;
    keyShortPressCallback[i] = NULL;
    keyLongPressCallback[i]  = NULL;
  }
  
  TIM_DeInit(KEY_TIM);
  RCC_APB1PeriphClockCmd(KEY_RCC_APB1Periph_TIM, DISABLE);
  
  NVIC_InitTypeDef NVIC_InitStructure = {0};
  
  NVIC_InitStructure.NVIC_IRQChannel                   = KEY_TIM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = KEY_TIM_IRQ_PreemptionPriority;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = KEY_TIM_IRQ_SubPriority;
  NVIC_InitStructure.NVIC_IRQChannelCmd                = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Get key status.
  * @param  [in] pin: That key.
  * @return Key status.
  */
KEY_Status KEY_GetStatus(KEY_Pin pin)
{
  KEY_Status status = KEY_NoPress;
  
  if(keyGetStatus[pin] != KEY_NoPress)
  {
    status            = keyGetStatus[pin];
    keyGetStatus[pin] = KEY_NoPress;
  }
  
  return status;
}

/**
  * @brief  Set key short press callback.
  * @param  [in] pin: That key.
  * @param  [in] fun: Function pointer.
  * @return None.
  */
void KEY_SetShortPressCallback(KEY_Pin pin, void (*fun)(void))
{
  keyShortPressCallback[pin] = (__IO void (*)(void))fun;
}

/**
  * @brief  Set key long press callback.
  * @param  [in] pin: That key.
  * @param  [in] fun: Function pointer.
  * @return None.
  */
void KEY_SetLongPressCallback(KEY_Pin pin, void (*fun)(void))
{
  keyLongPressCallback[pin] = (__IO void (*)(void))fun;
}

/**
  * @brief  Clear key short press callback.
  * @param  [in] pin: That key.
  * @param  [in] fun: Function pointer.
  * @return None.
  */
void KEY_ClearShortPressCallback(KEY_Pin pin)
{
  keyShortPressCallback[pin] = NULL;
}

/**
  * @brief  Clear key long press callback.
  * @param  [in] pin: That key.
  * @param  [in] fun: Function pointer.
  * @return None.
  */
void KEY_ClearLongPressCallback(KEY_Pin pin)
{
  keyLongPressCallback[pin] = NULL;
}

/**
  * @brief  This function handles TIM Handler.
  * @param  None.
  * @return None.
  */
void KEY_TIM_IRQHandler(void)
{
  if(TIM_GetITStatus(KEY_TIM, TIM_IT_Update) != RESET)
  {
    TIM_ClearITPendingBit(KEY_TIM, TIM_IT_Update);
    
    for(int i = 0; i < KEYn; i++)
    {
      if(GPIO_ReadInputDataBit(KEY_GPIO[i], KEY_GPIO_Pin[i]) == KEY_PRESS_STATUS)
      {
        keyInputLowCount[i]++;
        
        if(keyInputLowCount[i] > 1)
        {
          keyInputHighCount[i] = 0;
        }
        
        if(keyInputLowCount[i] > 100)
        {
          keyInputLowCount[i] = 0;
          keyStatus[i]        = KEY_LongPress;
          keyGetStatus[i]     = KEY_LongPress;
          
          if(keyLongPressCallback[i] != NULL)
          {
            keyLongPressCallback[i]();
          }
        }
      }
      else
      {
        keyInputHighCount[i]++;
        
        if(keyInputHighCount[i] > 1)
        {
          if((keyStatus[i] == KEY_NoPress) && (keyInputLowCount[i] > 1))
          {
            keyInputHighCount[i] = 0;
            keyInputLowCount[i]  = 0;
            keyStatus[i]         = KEY_ShortPress;
            keyGetStatus[i]      = KEY_ShortPress;
            
            if(keyShortPressCallback[i] != NULL)
            {
              keyShortPressCallback[i]();
            }
          }
          else
          {
            keyInputHighCount[i] = 0;
            keyInputLowCount[i]  = 0;
            keyStatus[i]         = KEY_NoPress;
          }
        }
      }
    }
  }
}
