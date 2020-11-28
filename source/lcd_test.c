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

  // Ensure LCD is initialized 
  // and display and cursor ON.
  lcd_init();
  lcd_display_ctrl (DISPLAY_ON | CURSOR_ON | BLINKING_ON);

  char c;
  uint8_t addr;


  do
    {
      c = USART_Receive();
      
      if (c == 127) // backspace and delete
        {        
          addr = lcd_read_addr();

          lcd_entry_mode_set (DECREMENT);
          if (0x40 == addr)
            lcd_set_ddram_addr(0x13);
          else if (0x14 == addr)
            lcd_set_ddram_addr(0x53);
          else if (0x54 == addr)
            lcd_set_ddram_addr(0x27);
          else
            lcd_cursor_shift (LEFT_SHIFT);

          lcd_write_data (' ');
          lcd_cursor_shift (RIGHT_SHIFT);
          lcd_entry_mode_set (INCREMENT);
        }

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

      else if (c == 0x08) // ctrl + 'h' = home.
      {
        lcd_return_home();
      } 
      
      else if (c == 0x03) // ctrl + 'c' = clear screen.
      {
        lcd_clear_display();
      }

      else if (c == 0x04) // ctrl + 'd' shift display
      {
        lcd_display_shift (RIGHT_SHIFT);
      }

      else if (c == 0x1B) // direction
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

      else // move foward and print character.
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
        
        addr = lcd_read_addr();
        print_str("\n\r addr = 0x");print_hex(addr);
    } 
  while (1);

  return 1;
}