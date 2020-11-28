/*
***********************************************************************************************************************
*                                                     AVR-LCD MODULE
*
* File    : LCD_SF.H
* Author  : Joshua Fain
* Target  : ATMega1280
* LCD     : Gravitech 20x4 LCD using HD44780 LCD controller
*
*
* DESCRIPTION:
* Contains the function definitions for special functions used to initiate specific behaviors from the LCD. These 
* functions require the functions in LCD_BASE.C/H. The functions here will call the relevant basic instruction function
* required to execute the requested behavior of the special function called here.
*
*
* FUNCTIONS:
*   (1)  uint8_t   lcd_read_addr(void)
*   (2)  void      lcd_cursor_shift (uint8_t direction)
*   (3)  void      lcd_display_shift (uint8_t direction)
*
*
*                                                       MIT LICENSE
*
* Copyright (c) 2020 Joshua Fain
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
* documentation files (the "Software"), to deal in the Software without restriction, including without limitation the 
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
* permit ersons to whom the Software is furnished to do so, subject to the following conditions: The above copyright 
* notice and this permission notice shall be included in all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE 
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************************
*/


#include <stdint.h>
#include <avr/io.h>
#include "../includes/lcd_base.h"
#include "../includes/lcd_sf.h"
#include "../includes/usart.h"
#include "../includes/prints.h"
#include <util/delay.h>



/*
***********************************************************************************************************************
 *
 *                                                      FUNCTIONS
 * 
***********************************************************************************************************************
*/



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                 READ ADDRESS COUNTER
 * 
 * Description : Reads the address counter by calling lcd_read_busy_and_addr() and returning the value with the busy
 *               flag bit cleared.
 * 
 * Arguments   : None
 * 
 * Returns     : Current value of the address counter.
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_read_addr (void)
{
  uint8_t addr;
  addr = lcd_read_busy_and_addr();
  return (0b01111111 & addr); // removes busy flag.
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                     SHIFT CURSOR
 * 
 * Description : Shift cursor one position in the specified direction.
 * 
 * Arguments   : Direction. RIGHT or LEFT
-----------------------------------------------------------------------------------------------------------------------
*/

void lcd_cursor_shift (uint8_t direction)
{
  lcd_cursor_display_shift (CURSOR_SHIFT | direction);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                     SHIFT DISPLAY
 * 
 * Description : Shift display one position in the specified direction.
 * 
 * Arguments   : Direction. RIGHT or LEFT
-----------------------------------------------------------------------------------------------------------------------
*/

void lcd_display_shift (uint8_t direction)
{
  lcd_cursor_display_shift (DISPLAY_SHIFT | direction);
}