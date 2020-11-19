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
* Header file for LCD.C. This is for use interfacing an AVR microcontroller and an LCD screen. This header file 
* defines macrose and declares the functions defined in LCD.C.
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


#ifndef LCD_H
#define LCD_H

// ---------------------------------
// Define Control Port and flags
// RS : 0 = Instruction Register, 1 = Data Register
// R/W: 0 = Write, 1 = Read
// Pulse enable to send command/instruction. Sent on 
#define CTRL_PORT             PORTC
#define REGISTER_SELECT       PC0
#define READ_WRITE            PC1
#define ENABLE                PC2

#define DATA_REGISTER         CTRL_PORT &= ~(1 << REGISTER_SELECT)
#define INST_REGISTER         CTRL_PORT |=  (1 << REGISTER_SELECT)

#define READ                  CTRL_PORT |=  (1 << READ_WRITE)
#define WRITE                 CTRL_PORT &= ~(1 << READ_WRITE)

#define ENABLE_HI             CTRL_PORT |=  (1 << ENABLE)
#define ENABLE_LO             CTRL_PORT &= ~(1 << ENABLE)


// Define Data Port and it's pins
#define DATA_PORT             PORTA
#define DB0                   PA0
#define DB1                   PA1
#define DB2                   PA2
#define DB3                   PA3
#define DB4                   PA4
#define DB5                   PA5
#define DB6                   PA6
#define DB7                   PA7


// LCD Instruction Flags
#define CLEAR_DISPLAY         0x01
#define RETURN_HOME           0x02
#define ENTRY_MODE_SET        0x04
#define DISPLAY_ON_OFF        0x08
#define CURSOR_DISPLAY_SHIFT  0x10
#define FUNCTION_SET          0x20
#define SET_CGRAM_ADDR        0x40
#define SET_DDRAM_ADDR        0x80

// LCD Instruction Specific Settings

// Entry Mode Settings
#define INCREMENT             0x02
#define DECREMENT             0x00
#define DISPLAY_SHIFT_DATA    0x01

// Display ON/OFF Control Settings
#define DISPLAY_ON            0x04
#define DISPLAY_OFF           0x00
#define CURSOR_ON             0x02
#define CURSOR_OFF            0x00
#define BLINKING_ON           0x01
#define BLINKING_OFF          0x00

// Cursor or Display Shift Settings
#define DISPLAY_SHIFT         0x08
#define CURSOR_SHIFT          0x00
#define RIGHT_SHIFT           0x04
#define LEFT_SHIFT            0x00

// Function Set Settings
#define DATA_LENGTH_8_BITS    0x10
#define DATA_LENGTH_4_BITS    0x00
#define TWO_LINES             0x08
#define ONE_LINE              0x00
#define FONT_5x10             0x04
#define FONT_5x8              0x00






// Basic Instructions
void 
lcd_clear_display();

void 
lcd_return_home();

void 
lcd_entry_mode_set(uint8_t setting);

void 
lcd_display_on_off_ctrl(uint8_t setting);

void 
lcd_cursor_display_shift(uint8_t setting);

void 
lcd_function_set(uint8_t setting);

void 
lcd_set_cgram_addr(uint8_t acg);

void 
lcd_set_ddram_addr(uint8_t add);

void 
lcd_write_data(uint8_t data);

uint8_t
lcd_read_data(void);



// Special Functions
void lcd_cursor_shift(uint8_t direction);
void lcd_display_shift(uint8_t direction);


void 
lcd_wait_busy();

void 
lcd_pulse_enable();

void
lcd_send_command(uint8_t cmd);

uint8_t
lcd_read_addr(void);


#endif // LCD_H