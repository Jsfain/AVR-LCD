/*
***********************************************************************************************************************
*                                                     AVR-LCD MODULE
*
* File    : LCD.H
* Version : 0.0.0.1 
* Author  : Joshua Fain
* Target  : ATMega1280
* LCD     : Gravitech 20x4 LCD using HD44780 LCD controller
*
*
* DESCRIPTION:
* 
*
* FUNCTIONS:
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
#include "../includes/lcd.h"
#include "../includes/usart.h"
#include "../includes/prints.h"
#include <util/delay.h>


void 
lcd_send_command (uint8_t cmd)
{
  DATA_PORT = cmd;
  _delay_ms(1);
  lcd_pulse_enable();
}


// Basic Instructions
void 
lcd_clear_display()
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (CLEAR_DISPLAY);
}


void 
lcd_return_home()
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (RETURN_HOME);
}


void 
lcd_entry_mode_set(uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (ENTRY_MODE_SET | setting);
}


void 
lcd_display_on_off_ctrl(uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (DISPLAY_ON_OFF | setting);
}


void 
lcd_cursor_display_shift(uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (CURSOR_DISPLAY_SHIFT | setting);
}


void 
lcd_function_set(uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (FUNCTION_SET | setting);
}


void 
lcd_set_cgram_addr(uint8_t acg)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (SET_CGRAM_ADDR | acg);
}


void 
lcd_set_ddram_addr(uint8_t add)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_command (SET_DDRAM_ADDR | add);
}


void 
lcd_write_data(uint8_t data)
{
  lcd_wait_busy();
  INST_REGISTER;
  WRITE;
  DATA_PORT = data;
  _delay_ms(1);
  lcd_pulse_enable(); 
}


uint8_t
lcd_read_data(void)
{
  uint8_t data = 0;
  lcd_wait_busy();
  DDRA = 0;
  INST_REGISTER;
  READ;
  _delay_ms(1);
  lcd_pulse_enable();
  data = PINA;
  DDRA = 0xFF;
  return data;
}

uint8_t
lcd_read_addr(void)
{
  uint8_t addr;
  DDRA = 0;
  DATA_REGISTER;
  READ;
  _delay_ms(5);
  CTRL_PORT |= (1 << ENABLE);
  _delay_ms(1);
  addr = PINA;
  DDRA = 0xFF;
  return addr;
}



// Special Functions
void lcd_cursor_shift(uint8_t direction)
{
  lcd_cursor_display_shift(CURSOR_DISPLAY_SHIFT | CURSOR_SHIFT | direction);
}

void lcd_display_shift(uint8_t direction)
{
  lcd_cursor_display_shift(CURSOR_DISPLAY_SHIFT | DISPLAY_SHIFT | direction);
}




void 
lcd_wait_busy()
{
  DDRA = 0;
  DATA_REGISTER;
  READ;
  _delay_ms(5);
  ENABLE_HI;
  int cnt = 0;
  do
    {
      cnt++;
      _delay_ms(1);
    } 
  while ((DATA_PORT & 0x80) && (cnt < 10));
  DDRA = 0xFF;
}


void 
lcd_pulse_enable(void)
{
  _delay_ms(0.5);
  ENABLE_HI;
  _delay_ms(0.5);
  ENABLE_LO;
}
