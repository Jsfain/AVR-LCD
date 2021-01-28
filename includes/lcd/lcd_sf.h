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
* Header file for LCD_SF.C. This file contains the prototypes for special functions for operating the LCD. These 
* functions require the functions in LCD_BASE.C/H which are used to call the particular basic instruction function
* that is required to execute the requested behavior in these special functions.
*
*
* FUNCTIONS:
*   (1)  void      lcd_cursor_shift (uint8_t direction)
*   (2)  void      lcd_display_shift (uint8_t direction)
*   (3)  void      lcd_read_addr(void)
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


#ifndef LCD_SF_H
#define LCD_SF_H

#include <avr/io.h>


/*
***********************************************************************************************************************
 *
 *                                                FUNCTION PROTOTYPES
 * 
***********************************************************************************************************************
*/


void 
lcd_cursor_shift (uint8_t direction);

void 
lcd_display_shift (uint8_t direction);

uint8_t
lcd_read_addr(void);


#endif // LCD_SF_H