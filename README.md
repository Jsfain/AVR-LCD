# AVR-LCD Module
Interface module for controlling an LCD screen with a AVR microcontroller.


## Technology
* LCD: [Gravitech 20x4 LCD using HD44780 LCD controller](https://www.gravitech.us/20chbllcd.html)
* TARGET: ATmega1280
* LANGUAGE: C
* [AVR-Toolchain](https://github.com/osx-cross/homebrew-avr) 9.3.0 , This includes: 
  * AVR-GCC 9.3.0
  * AVRDUDE 6.3

## Overview
The AVR-LCD interface is composed of a two source/header files - One that explicitely implements the basic instructions (LCD_BASE) available to the LCD, and the other (LCD_SF) implements a few simple, non-mode setting, behaviors/special functions based on functions available in LCD_BASE. Thus LCD_SF requires LCD_BASE to operate. An additional header is also included (LCD_ADDR.H) that provides a few macro definitions for the beggining and ending row position DDRAM addresses. 

### AVR-LCD Module
1. **LCD_BASE** - Required
    * This includes the functions that execute the basic instruction set available to the LCD controller. 
    * Several of the instructions require passing settings to dictate LCD functioning. These settings are defined in the macros in LCD_BASE.H and can be passed to their associated function/instruction as the argument. For example, the LCD controller's CURSOR OR DISPLAY SHIFT instruction will be executed by calling lcd_cursorDisplayShift(arg). To shift the display to the right, then 'arg' = 'DISPLAY | RIGHT'.

2. **LCD_SF** - Requires LCD_BASE
    * Includes functions to execute specific implementations of the LCD_BASE functions.
    * The functions called hear will only implement instructions that are non-mode setting.
    * More specifically, the functions included hear will only execute one-time actions on the LCD display/controller. For instance, lcd_rightShiftDisplay() can be called to shift the display one space to the right.

3. **LCD_ADDR.H**
    * The display rows are addressed as such:
      - row 1: DDRAM Address - 0x00 - 0x13
      - row 2: DDRAM Address - 0x40 - 0x53
      - row 3: DDRAM Address - 0x14 - 0x27
      - row 4: DDRAM Address - 0x54 - 0x67
    
    * Therefore the macros defined here are used to specify the address of the display rows' beginning and ending positions so that the cursor will move to the next row when it reaches the end of one row, rather than the position pointed at by the 'next' address.    
    * The macros in this header are not currently required by LCD_BASE or LCD_SF, but are provided for useful in specific programs that use this AVR-LCD module.

### Additional Required Files
The following source/header files are also used, but not necessarily required, depending on how the AVR-LCD module is implemented. These are included in the repository but maintained in [AVR-General](https://github.com/Jsfain/AVR-General.git)

1. PRINTS.H(C) : print integers (decimal, hex, binary) and strings to a terminal via the USART. 
    * In this module, PRINTS is only required for use if implementing the function ***lcd_printError()***, which will print error messages to a terminal (not the LCD display). If the AVR-LCD is being implemented, but not connected and/or printing to a terminal, then this function can be disabled and PRINTS.H/C not used. 
    * This function will include USART.H
2. USART.C(H)  : used to interface with the AVR's USART port. 
    * This is required by PRINTS above, but is also used in the test file included here to take keyboad input and send the characters to the LCD display.
    * This is not directly required by any of the AVR-LCD module files, but only used explicitely in the test file and by PRINTS.

## How to use
Copy the files and build/download the module using the AVR Toolchain. These are written for an ATmega1280 target, so if you are using a different target you may need to modify the code accordingly. This should only require modification of the PORT assignments, but I have not tested this.
 * The source files contain descriptions of each function available in the module.
 * LCD_TEST.C includes main() and can be used as an example for how to implement the module.
 * A *MAKE.SH* file is provided for reference, and you can see how I built the module from the source files and downloaded it to the AVR target. This would primarily be useful for non-Windows users without access to Atmel Studio.
 * Windows users should be able to just build/download the module from the source files using Atmel Studio (though I have not used this). Note, any paths (e.g. the includes) will need to be modified for compatibility.

## Who can use
Anyone. Use it. Modify it for your specific purpose/system. If you want, you can let me know if you found it helpful.

## License
[MIT](https://github.com/Jsfain/AVR-LCD/blob/master/LICENSE)

## Warnings / Disclaimers
1. Use at your own risk. This was developed for fun and so it is offered AS IS to anyone that wants to use it, look at it, or modify it for their purposes. There is no guarantee of operability. 
2. This module has only been tested on an ATmega1280 microcontroller.

## References
1. [AVR-General](https://github.com/Jsfain/AVR-General.git)
2. [AVR-Toolchain](https://github.com/osx-cross/homebrew-avr)
3. [Gravitech 20x4 LCD using HD44780 LCD controller](https://www.gravitech.us/20chbllcd.html)
3. Atmel ATmega640/V-1280/V-1281/V-2560/V-2561/V Datasheet
