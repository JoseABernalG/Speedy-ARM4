# Speedy-ARM4
An implementation on nucleo 144 board with an ARM M4 processor running at 80Mhz.

Version 1.0
This version already has initial functions used for encryption and decryption and some extra functions to extend the compatibility with the Nucleo 144 board.

This board has a STM32L4A6ZG with a speed of 80 MHz and implementation of AES cryptoprocessor, and connectivity via a USB-serial port.

The implementation of Speedy block cipher in C language is ready to be used and can encrypt and decrypt any data block. Besides, the input of data via serial is still in development.
