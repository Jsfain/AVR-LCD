# AVR-LCD Module
Interface module for controlling an LCD screen with a AVR microcontroller.


## Technology
* LCD: [Gravitech 20x4 LCD using HD44780 LCD controller](https://www.gravitech.us/20chbllcd.html)
* TARGET: ATmega1280
* LANGUAGE: C
* [AVR-Toolchain](https://github.com/osx-cross/homebrew-avr) 9.3.0 , This includes: 
  * AVR-GCC 9.3.0: required to compile/build the module.
  * AVRDUDE 6.3: required to download the program to the AVR.


## Overview
The AVR-LCD interface is composed of two source/header files. One that explicitely implements the basic instructions (LCD_BASE) available to the LCD, and the other (LCD_SF) implements specific behaviors/special functions by calling the basic functions with specific settings.

### AVR-LCD Module
1. LCD_BASE.C(H)
    * This includes the functions that execute the basic instruction set available to the LCD controller. Any instruction that also requires setting bits in the DATA_PORT to specify certain operation or functionality of the LCD allows passing these settings as flags as the argument.  For example, the LCD controller's CURSOR OR DISPLAY SHIFT instruction will be executed by calling lcd_cursor_display_shift(arg).  To shift the display to the right, then 'arg' = 'DISPLAY | RIGHT'.

2. LCD_SF.C(H)
    * Includes functions to execute specific instructions using the basic instructions in LCD_BASE.  For instance, lcd_display_shift_right() can be called to shift the display to the right, which will just call the basic command lcd_cursor_display_shift(DISPLAY | RIGHT).


### Additional Required Files
The following source/header files are also used, but not necessarily required, depending on how the AVR-LCD module is implemented. These are included in the repository but maintained in [AVR-General](https://github.com/Jsfain/AVR-General.git)

1. USART.C(H)   : used to interface with the AVR's USART port. This is used to print messages to a terminal, but also can be used to get characters typed on a keyboard and send them to the LCD screen.
2. PRINTS.H(C)  : required to print integers (decimal, hex, binary) and strings to a terminal via the USART. This is mostly used here to assist with debugging.


## How to use
Copy the files and build/download the module using the AVR Toolchain. These are written for an ATmega1280 target, so if using a different target you may need to modify the code accordingly.  
 * The source files contain descriptions of each function available in the module.
 * LCD_TEST.C includes main() and can be used as an example for how to implement the module.
 * A "MAKE.SH" file is provided for reference to see how I built the module from the source files and downloaded it to an ATmega1280 AVR target. This would primarily be useful for non-Windows users without access to Atmel Studio.
 * Windows users should be able to just build/download the module from the source files using Atmel Studio (though I have not used this). Note, any paths (e.g. the includes) will need to be modified for compatibility.


## Who can use
Anyone. Use it. Modify it for your specific purpose/system. If you want, you can let me know if you found it helpful.


## License
[MIT](https://github.com/Jsfain/AVR-LCD/blob/master/LICENSE)


## Warnings / Disclaimers
1. Use at your own risk. This was developed for fun and so it is offered "AS IS" to anyone that wants to use it, look at it, or modify it for their purposes. There is no guarantee of operability. 
2. This module has only been tested on an ATmega1280 microcontroller. It is expected to be easily portable to other AVR's through simple port (e.g. SPI, USART) reassignments, provided the resources exist, but also see 1.


## References
1. [AVR-General](https://github.com/Jsfain/AVR-General.git)
2. [AVR-Toolchain](https://github.com/osx-cross/homebrew-avr)
3. [Gravitech 20x4 LCD using HD44780 LCD controller](https://www.gravitech.us/20chbllcd.html)
3. Atmel ATmega640/V-1280/V-1281/V-2560/V-2561/V Datasheet
