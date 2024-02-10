# 2600MultiCart
Multi-Cartridge for the Atari 2600+

Using a Raspberry Pi PICO to emulate a 28C256 32KB EEPROM which is then interfaced to an Atari 2600 cartridge card.

The cart allows for 16 different cartridges of 4K, 8K, 16K and 32K sizes with standard bankswitching.  A reset switch is wired, which allows for hot-swapping cartridges on the 2600+.

A helper script 'make_romheaders.py' is provided that will read in ROMS from the roms/ directory and create the necesssary roms_XX.h headers.

![Alt Final Product in an Atari 2600+](images/IMG_6339.png)

