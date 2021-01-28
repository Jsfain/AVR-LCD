/*
 *                             TESTING FOR AVR-LCD
 *
 * File        : LCD_TEST.C
 * Author      : Joshua Fain
 * Host Target : ATMega1280
 * LCD         : Gravitech 20x4 LCD using HD44780 LCD controller
 * License     : MIT
 * Copyright (c) 2020, 2021
 *
 * Contains main(). Used to test the AVR-LCD Module. This test implements an 
 * interface that allows direct typing from a keyboard via the AVRs USART0 port
 * and redirects the characters entered to the LCD screen.
 *
 * NOTE:
 * Before simply sending a character to the LCD, the test program performs
 * several checks, such as checking to see wheter control or display 
 * character(s) were entered. It also tests to see if the address counter
 * should be adjusted. The way the LCD itself works, when the address counter
 * is pointing at the last address of a line, the next sequential address would
 * actually point to the first position 2 lines down, with the exception of the
 * last line. Thus, when at the end of a line and navigating to the next 
 * position, either by entering a character or pressing the right arrow key,
 * then the address counter will be updated by software to point to the first 
 * DDRAM address of the next line, rather than the next sequential DDRAM 
 * address. Similarly, for when moving backwards/decrementing. The opposite
 * should occur. 
 * 
 * The display lines map to the DDRAM addresses as follows:
 *
 * Line 1: DDRAM Address - 0x00 - 0x13
 * Line 2: DDRAM Address - 0x40 - 0x53
 * Line 3: DDRAM Address - 0x14 - 0x27
 * Line 4: DDRAM Address - 0x54 - 0x67
 *
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include "prints.h"
#include "usart0.h"
#include "lcd_base.h"
#include "lcd_sf.h"


int main(void)
{
  // usart required for character entry
  usart_init();

  // Ensure LCD is initialized.
  lcd_init();

  // Turn display and cursor on and blink the cursor 
  lcd_display_ctrl (DISPLAY_ON | CURSOR_ON | BLINKING_ON);


  char c;                              // for input characters
  uint8_t addr;                        // ddram address

  do
  {
    // get input character
    c = usart_receive();      

    //
    // If "backspace" is entered, this section will backspace, delete
    // character, backspace. Note this is set for use with a mac keyboard
    // where the backspace/delete key = 127 (delete).
    //
    if (c == 127)
    {        
      // set display to decrement address counter (AC)
      lcd_entry_mode_set (DECREMENT);

      // get current value of AC
      addr = lcd_read_addr();

      // If AC adjust is needed, set it accordingly, else LEFT_SHIFT.
      if (0x40 == addr)
        lcd_set_ddram_addr (0x13);
      else if (0x14 == addr)
        lcd_set_ddram_addr (0x53);
      else if (0x54 == addr)
        lcd_set_ddram_addr (0x27);
      else
        lcd_cursor_shift (LEFT_SHIFT);

      // Write space to display to clear value, and reset to INCREMENT mode.
      lcd_write_data (' ');
      lcd_cursor_shift(RIGHT_SHIFT);
      lcd_entry_mode_set(INCREMENT);
    }

    // if 'ENTER' is pressed, point AC to the first address of the next line.
    else if (c == '\r')
    {
      addr = lcd_read_addr();

      if (addr >= 0x00 && addr <= 0x13)
        lcd_set_ddram_addr (0x40);
      else if (addr >= 0x14 && addr <= 0x27)
        lcd_set_ddram_addr (0x54);
      else if (addr >= 0x40 && addr <= 0x53)
        lcd_set_ddram_addr (0x14);
      else if (addr >= 0x54 && addr <= 0x67)
        lcd_set_ddram_addr (0x00);
    }

    // if ctrl + 'h', return home. 
    else if (c == 0x08)
      lcd_return_home();
    
    // if ctrl + 'c', clear screen.
    else if (c == 0x03) 
      lcd_clear_display();

    // if ctrl + 'd', shift display.
    else if (c == 0x04) 
      lcd_display_shift (RIGHT_SHIFT);

    //
    // If right or left arrow is pressed then move cursor in that direction.
    // Note on mac keyboard, right and left arrows are 3 characters long and 
    // only differ in the third character. Right = 0x1B5B43, Left = 0x1B5B44.
    //
    else if (c == 0x1B)
    {
      c = usart_receive();
      if (c == 0x5B)
      {
        addr = lcd_read_addr();
        c = usart_receive();

        // Left arrow
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
        // right arrow
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

    // print character to LCD display
    else 
    {
      lcd_write_data(c);

      // AC adjustments if at the start of a display line.
      addr = lcd_read_addr();
      if (addr == 0x14)
        lcd_set_ddram_addr (0x40);
      else if (addr == 0x40)
        lcd_set_ddram_addr (0x54);
      else if (addr == 0x54)
        lcd_set_ddram_addr (0x14);
    }
  
  } 
  while (1);

  return 1;
}