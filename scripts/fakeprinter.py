#!/usr/bin/python

import sys
import time
import serial

fwlist = {
	'repetier' : """FIRMWARE_NAME:Repetier_XXX FIRMWARE_URL:XXX
			PROTOCOL_VERSION:1.0 MACHINE_TYPE:XXX EXTRUDER_COUNT:XXX 
			REPETIER_PROTOCOL:3\n\r""",
	'marlin' : """FIRMWARE_NAME:Marlin XXX SOURCE_CODE_URL:XXX
			PROTOCOL_VERSION:XXX MACHINE_TYPE:XXX EXTRUDER_COUNT:XXX
			UUID:XXX\n\r""",
	'sprinter' : """FIRMWARE_NAME: Sprinter Experimental PROTOCOL_VERSION:1.0
			MACHINE_TYPE:Mendel EXTRUDER_COUNT:1\n\r""",
	'teacup' : """FIRMWARE_NAME:Teacup FIRMWARE_URL:http://github.com/traumflug/Teacup_Firmware/
			PROTOCOL_VERSION:1.0 MACHINE_TYPE:Mendel EXTRUDER_COUNT:X
			TEMP_SENSOR_COUNT:X HEATER_COUNT:X\n\r""",
}

fwname = 'repetier'
if(len(sys.argv) == 1):
	print('Add an argument to run the correct firmware:')
	for fw in  fwlist:
		print('\t',fw)
	print('Default: ',fwname)
if(len(sys.argv) == 2):
	if(sys.argv[1] in fwlist):
		fwname = sys.argv[1]
		print('Firmware: ',fwname)

ser = serial.Serial('/dev/ttyVirtual2')

def check(msg):
	if(msg == 'M115'):
		return fwlist[fwname]
	return 'ok\n\r'

while(True):
	line = ser.readline().strip()
	print(line)
	ans = check(line.decode()).encode()
	print(ans)
	ser.write(ans)
	while(ser.inWaiting()<1):
		time.sleep(0.01)

