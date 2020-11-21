clear

#directory to store build/compiled files
buildDir=../untracked/build

#directory for source files
sourceDir=source

#make build directory if it doesn't exist
mkdir -p -v $buildDir


t=0.25
# -g = debug, -Os = Optimize Size
Compile=(avr-gcc -Wall -g -Os -mmcu=atmega1280 -c -o)
Link=(avr-gcc -Wall -g -mmcu=atmega1280 -o)
IHex=(avr-objcopy -j .text -j .data -O ihex)


echo -e ">> COMPILE: "${Compile[@]}" "$buildDir"/lcd_test.o " $sourceDir"/lcd_test.c"
"${Compile[@]}" $buildDir/lcd_test.o $sourceDir/lcd_test.c
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error compiling LCD_TEST.C"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Compiling LCD_TEST.C successful"
fi


echo -e ">> COMPILE: "${Compile[@]}" "$buildDir"/lcd_base.o " $sourceDir"/lcd_base.c"
"${Compile[@]}" $buildDir/lcd_base.o $sourceDir/lcd_base.c
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error compiling LCD_BASE.C"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Compiling LCD_BASE.C successful"
fi


echo -e "\n\r>> COMPILE: "${Compile[@]}" "$buildDir"/prints.o "$sourceDir"/prints.c"
"${Compile[@]}" $buildDir/prints.o $sourceDir/prints.c
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error compiling PRINTS.C"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Compiling PRINTS.C successful"
fi


echo -e "\n\r>> COMPILE: "${Compile[@]}" "$buildDir"/usart.o "$sourceDir"/usart.c"
"${Compile[@]}" $buildDir/usart.o $sourceDir/usart.c
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error compiling USART.C"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Compiling USART.C successful"
fi


echo -e "\n\r>> LINK: "${Link[@]}" "$buildDir"/lcd_test.elf "$buildDir"/lcd_test.o  "$buildDir"/lcd_base.o  "$buildDir"/usart.o "$buildDir"/prints.o "
"${Link[@]}" $buildDir/lcd_test.elf $buildDir/lcd_test.o $buildDir/lcd_base.o $buildDir/usart.o $buildDir/prints.o
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error during linking"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Linking successful. Output in LCD_TEST.ELF"
fi



echo -e "\n\r>> GENERATE INTEL HEX File: "${IHex[@]}" "$buildDir"/lcd_test.elf "$buildDir"/lcd_test.hex"
"${IHex[@]}" $buildDir/lcd_test.elf $buildDir/lcd_test.hex
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error generating HEX file"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "HEX file successfully generated. Output in LCD_TEST.HEX"
fi


echo -e "\n\r>> DOWNLOAD HEX FILE TO AVR"
echo "avrdude -p atmega1280 -c dragon_jtag -U flash:w:lcd_test.hex:i -P usb"
avrdude -p atmega1280 -c dragon_jtag -U flash:w:$buildDir/lcd_test.hex:i -P usb
status=$?
sleep $t
if [ $status -gt 0 ]
then
    echo -e "error during download of HEX file to AVR"
    echo -e "program exiting with code $status"
    exit $status
else
    echo -e "Program successfully downloaded to AVR"
fi