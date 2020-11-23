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
* Header file for LCD_BASE.C. This is for use interfacing an AVR microcontroller and an LCD screen. This header file 
* defines macrose and declares the functions defined in LCD_BASE.C.
*
*
* FUNCTIONS:
*   (1)  void      lcd_clear_display (void)
*   (2)  void      lcd_return_home (void)
*   (3)  uint8_t   lcd_entry_mode_set (uint8_t setting)
*   (4)  uint8_t   lcd_display_ctrl (uint8_t setting)
*   (5)  uint8_t   lcd_cursor_display_shift (uint8_t setting)
*   (6)  uint8_t   lcd_function_set (uint8_t setting)
*   (7)  uint8_t   lcd_set_cgram_addr (uint8_t acg)
*   (8)  uint8_t   lcd_set_ddram_addr (uint8_t add)
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


#ifndef LCD_BASE_H
#define LCD_BASE_H

#include <avr/io.h>
/*
***********************************************************************************************************************
 *
 *                                                       MACROS
 * 
***********************************************************************************************************************
*/


// -------------- Control Port & PINS

// The control port contains the 3 control PINS
// REGISTER_SELECT (RS), READ_WRITE (R/W), and ENABLE (E).
#define CTRL_PORT             PORTC
#define REGISTER_SELECT       PC0
#define READ_WRITE            PC1
#define ENABLE                PC2

// REGISTER_SELECT
#define DATA_REGISTER         CTRL_PORT &= ~(1 << REGISTER_SELECT) // RS = 0
#define INST_REGISTER         CTRL_PORT |=  (1 << REGISTER_SELECT) // RS = 1

// READ_WRITE
#define WRITE                 CTRL_PORT &= ~(1 << READ_WRITE) // R/W = 0
#define READ                  CTRL_PORT |=  (1 << READ_WRITE) // R/W = 1

// ENABLE
#define ENABLE_LO             CTRL_PORT &= ~(1 << ENABLE) // E = 0
#define ENABLE_HI             CTRL_PORT |=  (1 << ENABLE) // E = 1



// -------------- Data Port & PINS

#define DATA_PORT             PORTA
#define DB0                   PA0
#define DB1                   PA1
#define DB2                   PA2
#define DB3                   PA3
#define DB4                   PA4
#define DB5                   PA5
#define DB6                   PA6
#define DB7                   PA7



// -------------- LCD Instruction Flags

// These flags are used to select the instruction type
// to execute.  Most of these typese have accompanying
// instruction-sepecific settings as well that must
// be specified.

#define CLEAR_DISPLAY         0x01
#define RETURN_HOME           0x02
#define ENTRY_MODE_SET        0x04
#define DISPLAY_CTRL          0x08
#define CURSOR_DISPLAY_SHIFT  0x10
#define FUNCTION_SET          0x20
#define SET_CGRAM_ADDR        0x40
#define SET_DDRAM_ADDR        0x80



// -------------- LCD Instruction-Specific Setting Flags

// ENTRY_MODE_SET
#define INCREMENT             0x02
#define DECREMENT             0x00
#define DISPLAY_SHIFT_DATA    0x01

// DISPLAY_ON_OFF_CTRL
#define DISPLAY_ON            0x04
#define DISPLAY_OFF           0x00
#define CURSOR_ON             0x02
#define CURSOR_OFF            0x00
#define BLINKING_ON           0x01
#define BLINKING_OFF          0x00

// CURSOR_DISPLAY_SHIFT
#define DISPLAY_SHIFT         0x08
#define CURSOR_SHIFT          0x00
#define RIGHT_SHIFT           0x04
#define LEFT_SHIFT            0x00

// FUNCTION_SET
#define DATA_LENGTH_8_BITS    0x10
#define DATA_LENGTH_4_BITS    0x00
#define TWO_LINES             0x08
#define ONE_LINE              0x00
#define FONT_5x10             0x04
#define FONT_5x8              0x00


// Errors
#define LCD_INSTR_SUCCESS     0x00
#define INVALID_ARGUMENT      0x01


// Busy error codes returned by lcd_wait_busy();
#define BUSY_RESET_SUCCESS    0x02
#define BUSY_RESET_TIMEOUT    0x04



/*
***********************************************************************************************************************
 *
 *                                                FUNCTION PROTOTYPES
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
lcd_clear_display (void);



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   RETURN HOME
 * 
 * Description : Sets DDRAM address to 0 in the address counter. Returns dispaly to original position. DDRAM contents
 *               are not changed.  
-----------------------------------------------------------------------------------------------------------------------
*/

void 
lcd_return_home (void);



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
 * 
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_entry_mode_set (uint8_t setting);



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
 * 
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_display_ctrl (uint8_t setting);



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
 *
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_cursor_display_shift (uint8_t setting);



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
 * 
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_function_set (uint8_t setting);



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   SET CGRAM ADDRESS
 * 
 * Description : Set the CGRAM address. CGRAM data is sent and received after sending this instruction.
 * 
 * Argument    : acg     - byte which holds the 6-bit address the CGRAM pointer will point to. The lowest 6 bits in the
 *                         acg argument will be the address.
 * 
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t 
lcd_set_cgram_addr (uint8_t acg);



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                                   SET DDRAM ADDRESS
 * 
 * Description : Set the DDRAM address. DDRAM data is sent and received after sending this instruction.
 * 
 * Argument    : add     - byte which holds the 7-bit address the DDRAM pointer will point to. The lowest 7 bits in the
 *                         add argument will be the address.
 *
 * Returns     : LCD Error code
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_set_ddram_addr (uint8_t add);



/* 
-----------------------------------------------------------------------------------------------------------------------
 *                                        READ BUSY FLAG and ADDRESS COUNTER
 * 
 * Description : Reads the busy flag indicating internal operation is being performed and also reads address counter.
 * 
 * Returns     : byte     - the MSB (bit 7) is the Busy Flag. This will be set to 1 if controller is busy.  The lowest
 *                          7 bits (bits 0 to 6) will specify the value of the address counter.
 * 
 * Returns     : byte. Bit 7 is the Busy Flag and bits 0 to 6 are the address.
-----------------------------------------------------------------------------------------------------------------------
*/

uint8_t
lcd_read_busy_and_addr (void);



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
lcd_write_data (uint8_t data);



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
lcd_read_data (void);



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
lcd_wait_busy (void);



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
lcd_pulse_enable (void);



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
lcd_send_instruction (uint8_t cmd);


#endif // LCD_BASE_H