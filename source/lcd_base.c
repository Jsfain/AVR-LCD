/*
***********************************************************************************************************************
*                                                     AVR-LCD MODULE
*
* File    : LCD_BASE.H
* Author  : Joshua Fain
* Target  : ATMega1280
* LCD     : Gravitech 20x4 LCD using HD44780 LCD controller
*
*
* DESCRIPTION:
* Defines basic functions for interfacing with the LCD. The functions here are used to implement the basic instructions
* available to the HD44780. 'setting' arguments can be passed to the basic functions with the corresponding function
* requires setting certain bits to specify features, settings, or operation. These settings are available as MACRO
* flags defined in LCD_BASE.H.  Also included here are some non-basic, but required helper functions, for example, 
* lcd_wait_busy() and lcd_send_instruction().
*
*
* FUNCTIONS:
*   (1)  void      lcd_clear_display (void)
*   (2)  void      lcd_return_home (void)
*   (3)  void      lcd_entry_mode_set (uint8_t setting)
*   (4)  void      lcd_display_ctrl (uint8_t setting)
*   (5)  void      lcd_cursor_display_shift (uint8_t setting)
*   (6)  void      lcd_function_set (uint8_t setting)
*   (7)  void      lcd_set_cgram_addr (uint8_t acg)
*   (8)  void      lcd_set_ddram_addr (uint8_t add)
*   (9)  uint8_t   lcd_read_busy_and_address (void)
*   (10) void      lcd_write_data (uint8_t data)
*   (11) uint8_t   lcd_read_data (void)
*   (12) uint8_t   lcd_wait_busy (void)
*   (13) void      lcd_pulse_enable (void)
*   (14) void      lcd_send_instruction (uint8_t cmd)
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



// ***********************************     LCD Basic Instruction Functions     ***************************************



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                             CLEAR LCD SCREEN / DISPLAY
 * 
 * Description : Clears the display and sets DDRAM address to 0 in address counter.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_clear_display (void)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (CLEAR_DISPLAY);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   RETURN HOME
 * 
 * Description : Sets DDRAM address to 0 in the address counter. Returns dispaly to original position. DDRAM contents
 *               are not changed.  
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_return_home (void)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (RETURN_HOME);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                      SET CURSOR DIRECTION and DISPLAY SHIFT
 * 
 * Description : Sets the direction the cursor moves and specifies whether display will shift. These operations will be
 *               performed during data writes and reads.
 * 
 * Argument    : setting     - byte to hold the settings of the instruction-specific bits to send with the
 *                             ENTRY_MODE_SET instruction. The available settings are listed below.
 * 
 * Settings    : INCREMENT or DECREMENT    - Specifies whether the cursor will increment or decrement by 1 upon a data
 *                                           read or write.
 *             : DISPLAY_SHIFT_DATA        - Setting this flag will cause the display to shift upon data read or write. 
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_entry_mode_set (uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (ENTRY_MODE_SET | setting);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                            DISPLAY ON/OFF, CURSOR ON/OFF, BLINKING
 * 
 * Description : Turns the display ON or OFF. Turns the cursor ON or OFF. Turns cursor blinking ON or OFF.
 * 
 * Argument    : setting     - byte to hold the settings of the instruction-specific bits to send with the DISPLAY_CTRL
 *                             instruction. The available settings are listed below.
 * 
 * Settings    : DISPLAY_ON or DISPLAY_OFF     - Specify whether display will be ON or OFF.
 *             : CURSOR_ON or CURSOR_OFF       - Specify whether cursor will be ON or OFF. 
 *             : BLINKING_ON or BLINKING_OFF   - Specify whether cursor blinking will be turned ON or OFF. 
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_display_ctrl (uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (DISPLAY_CTRL | setting);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   MOVE CURSOR or DISPLAY
 * 
 * Description : Immediately moves the cursor or the display to the right or left depending on the settings passed as
 *               the argument. It does not require data read or write in order to move the cursor or display, and it 
 *               does not matter what the ENTRY_MODE_SET settings are.
 * 
 * Argument    : setting     - byte to hold the settings of the instruction-specific bits to send with the
 *                             CURSOR_DISPLAY_SHIFT instruction. The available settings are listed below.
 * 
 * Settings    : CURSOR_SHIFT or DISPLAY_SHIFT   - Specify whether display or cursor will be shifted. 
 *             : RIGHT_SHIFT or LEFT_SHIFT       - Specify which direction the cursor or display will be shifted.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_cursor_display_shift (uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (CURSOR_DISPLAY_SHIFT | setting);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                        DATA LENGTH, DISPLAY LINES, CHARACTER FONT
 * 
 * Description : Sets the interface data length. Sets the number of display lines. Sets the character font.
 * 
 * Argument    : setting     - byte to hold the settings of the instruction-specific bits to send with the FUNCTION_SET
 *                             instruction. The available settings are listed below.
 * 
 * Settings    : DATA_LENTH_8_BITS or DATA_LENTH_4_BITS      - Specify whether to operate in 8-bit or 4-bit mode. 
 *             : TWO_LINES or ONE_LINE                       - Specify number of display lines.
 *             : FONT_5x10 or FONT_5x8                       - Specify characters are 5x10 or 5x8 dots.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_function_set (uint8_t setting)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (FUNCTION_SET | setting);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   SET CGRAM ADDRESS
 * 
 * Description : Set the CGRAM address. CGRAM data is sent and received after sending this instruction.
 * 
 * Argument    : acg     - byte which holds the 6-bit address the CGRAM pointer will point to. The lowest 6 bits in the
 *                         acg argument will be the address.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_set_cgram_addr (uint8_t acg)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (SET_CGRAM_ADDR | acg);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   SET DDRAM ADDRESS
 * 
 * Description : Set the DDRAM address. DDRAM data is sent and received after sending this instruction.
 * 
 * Argument    : add     - byte which holds the 7-bit address the DDRAM pointer will point to. The lowest 7 bits in the
 *                         add argument will be the address.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_set_ddram_addr (uint8_t add)
{
  lcd_wait_busy();
  DATA_REGISTER;
  WRITE;
  lcd_send_instruction (SET_DDRAM_ADDR | add);
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                        READ BUSY FLAG and ADDRESS COUNTER
 * 
 * Description : Reads the busy flag indicating internal operation is being performed and also reads address counter.
 * 
 * Returns     : byte     - the MSB (bit 7) is the Busy Flag. This will be set to 1 if controller is busy.  The lowest
 *                          7 bits (bits 0 to 6) will specify the value of the address counter.
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_read_busy_and_addr (void)
{
  uint8_t busy_addr;
  DDRA = 0;
  DATA_REGISTER;
  READ;
  CTRL_PORT |= (1 << ENABLE);
  _delay_ms(1);
  busy_addr = PINA;
  _delay_ms(1);
  DDRA = 0xFF;
  return busy_addr;
}




/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                           WRITE DATA TO DDRAM or CGRAM
 * 
 * Description : Write data to the DDRAM or CGRAM at location pointed to by the address counter.  Which RAM is written
 *               to will be determined by the most recent "set address" instruction that was sent (i.e. SET_DDRAM_ADDR
 *               or SET_CGRAM_ADDR)
 * 
 * Argument    : data    - byte of data that will be written to the DDRAM or CGRAM at the location pointed to by the 
 *                         address counter.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_write_data (uint8_t data)
{
  lcd_wait_busy();
  INST_REGISTER;
  WRITE;
  DATA_PORT = data;
  _delay_ms(1);
  lcd_pulse_enable(); 
}



/*
-----------------------------------------------------------------------------------------------------------------------
 *                                           READ DATA FROM DDRAM or CGRAM
 * 
 * Description : Read data from the DDRAM or CGRAM at the location pointed to by the address counter.  Which RAM is 
 *               written to will be determined by the most recent "set address" instruction that was sent (i.e. 
 *               SET_DDRAM_ADDR or SET_CGRAM_ADDR)
 * 
 * Returns    : the value of the byte in either CGRAM or DDRAM pointed at by the address counter.
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_read_data (void)
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



// ***************************************     Required helper functions     ******************************************



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                             WAIT FOR BUSY FLAG TO RESET
 * 
 * Description : Use this function to poll the busy flag and returns after it has been reset.
 * 
 * Arguments   : None
 * 
 * Returns     : Error code.  BUSY_RESET_SUCCESS if the busy flag was found to be reset and the interface is 
 *               ready to receive the next command.  BUSY_RESET_TIMEOUT if the flag does not reset after a certain
 *               timeout period.
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_wait_busy (void)
{
  DDRA = 0;
  DATA_REGISTER;
  READ;
  _delay_ms(5);
  ENABLE_HI;
  uint8_t cnt = 0;
  do
    {
      cnt++;
      _delay_ms(1);
      if (cnt > 254)
        {
          DDRA = 0xFF;
          return BUSY_RESET_TIMEOUT;
        }
    } 
  while (DATA_PORT & 0x80);
  DDRA = 0xFF;
  return BUSY_RESET_SUCCESS;
}


/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                  PULSE ENABLE PIN
 * 
 * Description : In order to execute an instructions the enable pin must transition from high to low.  This function
 *               sets the enable pin high and then low and so should be called once all the other necessary pins have 
 *               been set according to the desired instruction and settings to execute.  
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_pulse_enable (void)
{
  _delay_ms(0.5);
  ENABLE_HI;
  _delay_ms(0.5);
  ENABLE_LO;
}



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                              SEND INSTRUCTION TO LCD
 * 
 * Description : This function is called by the basic instruction functions to actually set the necessary pins for the
 *               instruction according to the argument
 * 
 * Arguments   : None
 * 
 * Returns     : Error code.  BUSY_RESET_SUCCESS if the busy flag was found to be reset and the interface is 
 *               ready to receive the next command.  BUSY_RESET_TIMEOUT if the flag does not reset after a certain
 *               timeout period.
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_send_instruction (uint8_t inst)
{
  DATA_PORT = inst;
  _delay_ms(1);
  lcd_pulse_enable();
}