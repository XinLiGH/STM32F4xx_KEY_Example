/**
  ******************************************************************************
  * @file    main.c
  * @author  XinLi
  * @version v1.0
  * @date    24-October-2017
  * @brief   Main program body.
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
#include "main.h"
#include "KEY.h"

/* Macro definitions ---------------------------------------------------------*/
/* Type definitions ----------------------------------------------------------*/
/* Variable declarations -----------------------------------------------------*/
/* Variable definitions ------------------------------------------------------*/
static __IO int key1ShortPressCount = 0;
static __IO int key1LongPressCount  = 0;
static __IO int key2ShortPressCount = 0;
static __IO int key2LongPressCount  = 0;

/* Function declarations -----------------------------------------------------*/
static void key1PressCallback(KEY_Status status);

/* Function definitions ------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None.
  * @return None.
  */
int main(void)
{
  KEY_Init(KEY_Pin1);
  KEY_Init(KEY_Pin2);
  
  KEY_SetPressCallback(KEY_Pin1, key1PressCallback);
  
  for(;;)
  {
    KEY_Status status = KEY_GetStatus(KEY_Pin2);
    
    if(status != KEY_NoPress)
    {
      if(status == KEY_ShortPress)
      {
        key2ShortPressCount++;
      }
      else
      {
        key2LongPressCount++;
      }
    }
  }
}

/**
  * @brief  Key 1 press callback.
  * @param  None.
  * @return None.
  */
static void key1PressCallback(KEY_Status status)
{
  if(status == KEY_ShortPress)
  {
    key1ShortPressCount++;
  }
  else if(status == KEY_LongPress)
  {
    key1LongPressCount++;
  }
}
