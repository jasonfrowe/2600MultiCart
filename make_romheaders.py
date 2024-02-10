#!/usr/bin/python3

import os
import numpy as np

# Change this array to select which ROMS you want
romlist = [15, 13, 52, 27, 56, 50, 57, 35, 30, 20, 67, 14, 43, 40, 6, 60]

romdir = "roms"
files = os.listdir(romdir)

nfiles = len(files)
for i in range(nfiles):
    print(i,files[i])

nroms = len(romlist)

print(" ")
for i in range(nroms):
    print(files[romlist[i]])   
    f = open(romdir+"/"+files[romlist[i]], 'rb')
    rom = f.read()
    
    romsize = len(rom)
    
    #Double up for 2K roms
    if romsize == 2048:
        #print('2K rom..',romsize,len(rom))
        rom_new = np.zeros(4096)
        for ii in range(romsize):
            rom_new[ii] = rom[ii]
            rom_new[ii+2048] = rom[ii]
        rom = np.copy(rom_new)
        romsize = 4096
        
    
    fheader = "roms_"+"{:02d}".format(i)+".h"
    
    fout = open(fheader, "w")
    
    fout.write('//'+"{:04d}".format(i)+" "+files[romlist[i]]+"\n")
    fout.write(' '+"\n")
    fout.write('uint romsize_in = '+str(romsize)+';'+"\n")
    for i in range(0,romsize,16):
        romstring = " "
        for j in range(16):
            romstring = romstring + str(rom[i+j]) + ", "
        if i == 0:
            fout.write('uint8_t rom_contents_in['+str(romsize)+'] = {'+romstring+"\n")
        elif i+j+1 == romsize:
            romstring = " "
            for j in range(15):
                romstring = romstring + str(rom[i+j]) + ", "
            romstring = romstring + str(rom[i+15]) + "};"
            fout.write(romstring+"\n")
        else:
            fout.write(romstring+"\n")
    
    fout.close()
    
    f.close()
