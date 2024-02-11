#!/usr/bin/env python3

import numpy as np
from paho.mqtt import client as mqtt_client

broker ='pi5.home.arpa'
port = 1883
client_id = f'python-mqtt-pi5'
client = mqtt_client.Client(client_id)
client.connect(broker, port)


romdir = "roms"
romfile = "Frostbite (USA).a26"

f = open(romdir+"/"+romfile, 'rb')
rom = f.read()
f.close()


hexstring = ''.join('{:02x}'.format(x) for x in rom[0:1000])
#print(hexstring)

try:
	topic=f'pico_w/recv'
	msg = 'hello'
	result = client.publish(topic,hexstring)

except:
    print('Connection Error')

