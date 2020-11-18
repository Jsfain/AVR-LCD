#ifndef LCD_H
#define LCD_H

#define CTRL_PORT           PORTC
#define REGISTER_SELECT     0x01 // Masks for 3 control port bits. RS  - 0 = Instruction Register, 1 = Data Register
#define READ_WRITE          0x02 // R/W - 0 = Write, 1 = Read
#define ENABLE              0x04


#define DB_PORT               PORTA
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


void 
lcd_wait_busy();

void 
lcd_pulse_enable();

void
lcd_send_command(uint8_t cmd);

uint8_t
lcd_read_addr(void);



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

#endif // LCD_H