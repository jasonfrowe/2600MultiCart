# 2600MultiCart
Multi-Cartridge for the Atari 2600+

Using a Raspberry Pi PICO to emulate a 28C256 32KB EEPROM which is then interfaced to an Atari 2600 cartridge card.

The cart allows for 16 different cartridges of 4K, 8K, 16K and 32K sizes with standard bankswitching.  A reset switch is wired, which allows for hot-swapping cartridges on the 2600+.

A helper script 'make_romheaders.py' is provided that will read in ROMS from the roms/ directory and create the necesssary roms_XX.h headers.

![Alt Final Product in an Atari 2600+](images/IMG_6339.png)

### Required Materials

| Part | Description | Mouser Part Number |  
| ----- | ----------- | -------------------|   
| Raspberry Pi Pico | Stable build uses the non-wifi version |  358-SC0915 |  
| Circuit board   | Gerber and Drill files are located in the gerbers folder |   
| SN74LVC245AN x2 | Bus Transceivers Tri-State Octal Bus | 595-SN74LVC245AN |  
| 10 K ohm resistor x 4 | Generic resistors | N/A |   
| 1N4007 diode     | Diode | N/A |   
| Sockets and headers | These are optional, but recommended -- and order extras!| 571-1-2199299-2  |  
| Button switch | I used an Omron B3F, but pick what you like. | N/A |
| Dip switches | 4 bank needed, again pick what you like.  | 655-2454982-3 | 

