#include <stdint.h>
#include <avr/io.h>
#include "../includes/prints.h"
#include "../includes/usart.h"
#include "../includes/lcd_base.h"
#include <util/delay.h>


int main(void)
{
  USART_Init();

  print_str("\n\rhello");

  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~ENABLE;
  CTRL_PORT &= ~READ_WRITE;

  CTRL_PORT = 0;

  DDRA = 0xFF;
  DDRC = 0xFF;
  _delay_ms(16);


  lcd_function_set (DATA_LENGTH_8_BITS | TWO_LINES);
  lcd_entry_mode_set (INCREMENT);
  lcd_clear_display();
  lcd_display_ctrl (DISPLAY_ON | CURSOR_ON | BLINKING_OFF);

  lcd_return_home();

  char c;

  do
  {
    c = USART_Receive();
    //USART_Transmit(c);
    if (c == 127)
      {
        lcd_cursor_shift(LEFT_SHIFT);
        lcd_entry_mode_set(DECREMENT);
        lcd_write_data(' ');
        lcd_cursor_shift(RIGHT_SHIFT);
        lcd_entry_mode_set(INCREMENT);
      }
    else
    {
      lcd_write_data(c);
    }
    
    if (lcd_read_addr() == 0x14)
    {
      lcd_set_ddram_addr(0x40);
    }
    else if (lcd_read_addr() == 0x40)
    {
      lcd_set_ddram_addr(0x54);
    }
    else if (lcd_read_addr() == 0x54)
    {
      lcd_set_ddram_addr(0x14);
    }

    //print_str("\n\r Address = 0x"); print_hex(lcd_read_addr());
    
  } while (c != '\r');
  
  lcd_return_home();

  
  for(uint8_t k = 0; k < 40; k++)
  {
    _delay_ms(1000);
    lcd_display_shift(RIGHT_SHIFT);
  }
  



  return 1;
}