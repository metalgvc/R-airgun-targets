targets lift project

Used hardware:
- Atmega8 - microcontroller
- 74HC595 - shift register
- seven-segment four-position indicator (cathode)


Compile and copy to VB shared folder to testing in Proteus (Win10):
make && cp main.hex ~/VirtualboxSharedFolder


-------

Compile:
$ make

Flash
$ make flash

Flash FUSES
$ make fuse

--------------------------------------------
used SPI interface & shift register for controll seven-segment indicator
