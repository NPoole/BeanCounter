@echo BeanCounter Firmware Programming Script v01

:begin

pause

start /B /wait .\avrdude -C.\avrdude.conf -v -pattiny861 -cusbtiny -B8 -e -Uefuse:w:0xFF:m -Uhfuse:w:0b11010111:m -Ulfuse:w:0xF1:m -Uflash:w:.\empty_all.hex:i 

start /B /wait .\avrdude -C.\avrdude.conf -v -pattiny861 -cusbtiny -B8 -Uflash:w:.\BeanCounter_V10.ino.hex:i 

GOTO begin