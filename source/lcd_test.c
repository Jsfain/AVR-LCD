/*
***********************************************************************************************************************
*                                               TESTING FOR AVR-LCD MODULE
*
* File    : LCD_TEST.C
* Author  : Joshua Fain
* Target  : ATMega1280
* LCD     : Gravitech 20x4 LCD using HD44780 LCD controller
*
*
* DESCRIPTION:
* Contains main(). Used to test the AVR-LCD Module.  This test implements an interface that allows direct typing from
* a terminal via USART port to the LCD screen.
*
* 
* NOTE:
* The test program will adjust the address counter when it reaches the last address of a line. In this case, when 
* navigating to the next position, either by entering a character or pressing the right arrow key, then the address 
* counter will be updated to point to the first DDRAM address of the next line, rather than the next chrnological 
* DDRAM address. Similarly, for when moving backwards/decrementing. This is necessary because the display lines map to
* the DDRAM addresses as follows:
*
* Line 1: DDRAM Address - 0x00 - 0x13
* Line 2: DDRAM Address - 0x40 - 0x53
* Line 3: DDRAM Address - 0x14 - 0x27
* Line 4: DDRAM Address - 0x54 - 0x67
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
#include "../includes/prints.h"
#include "../includes/usart.h"
#include "../includes/lcd_base.h"
#include "../includes/lcd_sf.h"
#include <util/delay.h>




int main(void)
{
  USART_Init();

  // Ensure LCD is initialized.
  lcd_init();

  lcd_display_ctrl (DISPLAY_ON | CURSOR_ON | BLINKING_OFF);

  char c;  
  uint8_t addr;

  do
    {
      c = USART_Receive();      

      // If backspace is pressed then this sections will perform
      // backspace and delete the previous character.
      // Backspace on Mac is 127.
      if (c == 127)
        {        
          // set display to decrement 
          // AC when character entered. 
          lcd_entry_mode_set (DECREMENT);

          // load current value of AC to see if it
          // is pointing to the beginning of a line.
          addr = lcd_read_addr();

          // adjust AC if address is at beginning of a line.
          // else just perform a LEFT_SHIFT.
          if (0x40 == addr)
            lcd_set_ddram_addr(0x13);
          else if (0x14 == addr)
            lcd_set_ddram_addr(0x53);
          else if (0x54 == addr)
            lcd_set_ddram_addr(0x27);
          else
            lcd_cursor_shift (LEFT_SHIFT);

          // Write ' ' to location to clear it's value,
          // and then reset to INCREMENT mode.
          lcd_write_data (' ');
          lcd_cursor_shift (RIGHT_SHIFT);
          lcd_entry_mode_set (INCREMENT);
        }

      // if 'ENTER' is pressed, then point AC to the first
      // address of the next line. 
      else if (c == '\r')
      {
        addr = lcd_read_addr();

        if ((addr >= 0x00) && (addr <= 0x13))
          lcd_set_ddram_addr (0x40);
        else if ((addr >= 0x14) && (addr <= 0x27))
          lcd_set_ddram_addr (0x54);
        else if ((addr >= 0x40) && (addr <= 0x53))
          lcd_set_ddram_addr (0x14);
        else if ((addr >= 0x54) && (addr <= 0x67))
          lcd_set_ddram_addr (0x00);
      }

      // if ctrl + 'h' is entered then return home. 
      else if (c == 0x08)
      {
        lcd_return_home();
      } 

      // if ctrl + 'c' is entered then clear screen.
      else if (c == 0x03) 
      {
        lcd_clear_display();
      }

      // if ctrl + 'd' is entered then shift display.
      // Note that this will not adjust for address
      // counter location.
      else if (c == 0x04) 
      {
        lcd_display_shift (RIGHT_SHIFT);
      }

      // if right- or left-arrow is pressed then
      // move cursor in the indicated direction.
      else if (c == 0x1B)
      {
        addr = lcd_read_addr();
        print_str("\n\r addr -> = 0x");print_hex(addr);
        c = USART_Receive();
        if (c == 0x5B)
          {
            c = USART_Receive();
            if (c == 0x44)
              {
                if (0x40 == addr)
                  lcd_set_ddram_addr(0x13);
                else if (0x14 == addr)
                  lcd_set_ddram_addr(0x53);
                else if (0x54 == addr)
                  lcd_set_ddram_addr(0x27);
                else
                  lcd_cursor_shift (LEFT_SHIFT);
              }
            else if (c == 0x43)
              {
                if (addr == 0x13)
                  lcd_set_ddram_addr(0x40);
                else if (addr == 0x53)
                  lcd_set_ddram_addr(0x14);
                else if (addr == 0x27)
                  lcd_set_ddram_addr(0x54);
                else
                  lcd_cursor_shift (RIGHT_SHIFT);
              }
          }
      } 

      // print character
      else 
        {
          lcd_write_data(c);

          addr = lcd_read_addr();

          if (addr == 0x14)
            lcd_set_ddram_addr(0x40);
          else if (addr == 0x40)
            lcd_set_ddram_addr(0x54);
          else if (addr == 0x54)
            lcd_set_ddram_addr(0x14);
        }
    } 
  while (1);

  return 1;
}