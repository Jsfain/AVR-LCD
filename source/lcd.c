#include <stdint.h>
#include <avr/io.h>
#include "../includes/lcd.h"
#include "../includes/usart.h"
#include "../includes/prints.h"
#include <util/delay.h>


void 
lcd_send_command (uint8_t cmd)
{
  DB_PORT = cmd;
  _delay_ms(1);
  lcd_pulse_enable();
}


// Basic Instructions
void 
lcd_clear_display()
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (CLEAR_DISPLAY);
}


void 
lcd_return_home()
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (RETURN_HOME);
}


void 
lcd_entry_mode_set(uint8_t setting)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (ENTRY_MODE_SET | setting);
}


void 
lcd_display_on_off_ctrl(uint8_t setting)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (DISPLAY_ON_OFF | setting);
}


void 
lcd_cursor_display_shift(uint8_t setting)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (CURSOR_DISPLAY_SHIFT | setting);
}


void 
lcd_function_set(uint8_t setting)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (FUNCTION_SET | setting);
}


void 
lcd_set_cgram_addr(uint8_t acg)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (SET_CGRAM_ADDR | acg);
}


void 
lcd_set_ddram_addr(uint8_t add)
{
  lcd_wait_busy();
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  lcd_send_command (SET_DDRAM_ADDR | add);
}


void 
lcd_write_data(uint8_t data)
{
  lcd_wait_busy();
  CTRL_PORT |= REGISTER_SELECT;
  CTRL_PORT &= ~READ_WRITE;
  DB_PORT = data;
  _delay_ms(1);
  lcd_pulse_enable(); 
}


uint8_t
lcd_read_data(void)
{
  uint8_t data = 0;
  lcd_wait_busy();
  DDRA = 0;
  CTRL_PORT |= REGISTER_SELECT;
  CTRL_PORT |= READ_WRITE;
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
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT |= READ_WRITE;
  _delay_ms(5);
  CTRL_PORT |= ENABLE;
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




///////////


void 
lcd_wait_busy()
{
  DDRA = 0;
  CTRL_PORT &= ~REGISTER_SELECT;
  CTRL_PORT |= READ_WRITE;
  _delay_ms(5);
  CTRL_PORT |= ENABLE;
  int cnt = 0;
  do
    {
      cnt++;
      _delay_ms(1);
      //print_str("\n\r PINA = ");print_bin(PINA);
    } 
  while ((DB_PORT & 0x80) && (cnt < 10));
  DDRA = 0xFF;
}


void 
lcd_pulse_enable(void)
{
  _delay_ms(0.5);
  CTRL_PORT |= ENABLE;
  _delay_ms(0.5);
  CTRL_PORT &= ~ENABLE;
}
