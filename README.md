# 2600MultiCart

**Disclaimer: Codes, PCB designs are provided for entertainment use only.  You assume all risk of assembly and usage.  There is the potential to damage your console if mistakes are made.**

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
| 10 K ohm resistor x 2 | Generic resistors | N/A |   
| 1N4007 diode     | Diode | N/A |   
| Sockets and headers | These are optional, but recommended -- and order extras!| 571-1-2199299-2  |  
| Button switch | I used an Omron B3F, but pick what you like. | N/A |
| Dip switches | 4 bank needed, again pick what you like.  | 655-2454982-3 | 

### Build Software

1. Place at least 16 dumped ROMs into the roms folder. These files should be .bin or .a26 format.
   - only 2K, 4K, 8K (F8), 16K (F6) and 32K (F4) ROMs are currently supported at the moment.
2. Run the `make_romheaders.py` script to generate `roms_XX.h` files
   - the first 16 ROMs in the folder will be used.  If you want to select different roms you need to line 7 of `make_romheaders.py` to reflect your choices
3. Make sure you have the C/C++ PICO SDK setup and the `PICO_SDK_PATH` environment variable points to your install location
4. `cd build`
5. `cmake .. -DPICO_BOARD=pico`
6. `make pico_rom` (note that just running make will attempt to also create the unstable WIFI rom) 
   
If everything worked you should have a file called `pico_rom.uf2` in the build folder.  

7. Hold down the `BOOTSEL` button and attach your Raspberry Pi Pico to your computer with a micro-usb cable.  Release the `BOOTSEL` button.
8. Copy `pico_rom.uf2` to the `RPI-RP2` folder.  If successful, the Pico will disconnect from your computer and start running the code.  The on-board LED will light up if everything is working.

### Order the board.

9. Create the board with a service such as PCBWay.  The board has dimensions 96.52 x 71.12 mm.  Two-layers and 1.6mm thickness is fine.
10. I changed the Solder mask to Red, but the default Green is fine.
11. Select Yes for Edge-connector.
12. If you don't want to spend much money, the HASL lead or HASL lead-free are okay.  ENIG is also a good choice, although more expensive.  
13. I also included a Bevel of 30 degrees
14. You can now Save to Cart
15. Upload the GerberFiles.  For convience a ZIP file can be found in the `gerbers` folder.
16. Wait for Review, make your payment and wait.
17. You may encounter 2 engineering questions.
    - removal of some material for the Bevel (this is okay)
    - removal of solder mask around the edge-connector (this is okay)
18. Wait for your board.

### Build the board.

19. Get your soldering iron and attach the Pico, sockets, diode and resistors.  Place these components on the side marked `PicoMultiCart`.
20. Now you have a choice.  The dip-switch and reset-button can be placed facing either side of the PCB.  I decided to have these components on the side that faces you when inserting the cart.
21. Note: The resistor at R3 connects to the LED in the latest revision.  Neither of these need to be populated at the moment.  Once a cart is ready, the led will be used to indicate power.

### Print a case

22. Use your 3D printer to print the included case. Note that supports and some sanding may be rquired.

### Play some games

23. Line up the cart with the pins on the 2600+ and insert the MultiCart.  Make sure the orientation is correct.  The side with the Pico and ICs should face away from you if you are in front of your console.
24. Turn on the Atari-2600+.  A game should load
25. To change games, simply change the dip-switches, then hit the `reset` button.  A new game should load.

### In Development

26. Wifi with a Pico-W and MQTT to transfer ROMs.
   - current WIFI build is unstable.  ROM timing needs additional work.
27. Making a better case