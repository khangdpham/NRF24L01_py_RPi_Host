
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import sys
import time
import spidev

# Usage  python send_rf.py <device#> <int>
try:
	GPIO.setmode(GPIO.BCM)
	Devices = {
				'host'	 : [0xf0,0xf0,0xf0,0xf0,0xe1] ,
				'device1' :[0xe0,0xe0,0xe0,0xe0,0xe1] ,
				'device2' :[0xe0,0xe0,0xe0,0xe0,0xe2] ,
				'device3' :[0xe0,0xe0,0xe0,0xe0,0xe3] ,
				'device4' :[0xe0,0xe0,0xe0,0xe0,0xe4] ,
				'device5' :[0xe0,0xe0,0xe0,0xe0,0xe5] ,
			}

	radio = NRF24(GPIO,spidev.SpiDev())
	radio.begin(0,25)
	radio.setPayloadSize(32)
	radio.setChannel(0x4c)
	radio.setDataRate(NRF24.BR_1MBPS)
	radio.setPALevel(NRF24.PA_MAX)
	
	# Need compliance
	#radio.setAutoAck(True)
	#radio.enableDynamicPayloads()
	radio.enableAckPayload()
	
	buf =  [int(sys.argv[2])]
	dev =  str(sys.argv[1])
	
	radio.openWritingPipe(Devices[dev])
	radio.openReadingPipe(1,Devices['host'])
	radio.printDetails()

	radio.stopListening()
	radio.write(buf)
	print "Sent : " , buf
	radio.startListening()
except Exception as ex:
	print ex
	print "Usage: sudo python send_rf.py <device#> <int>"
finally:
	# Clean up GPIO if used
	GPIO.cleanup()
	
	
	
