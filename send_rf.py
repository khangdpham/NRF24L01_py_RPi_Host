
import RPi.GPIO as GPIO
from lib_nrf24 import NRF24
import sys
import time
import spidev


GPIO.setmode(GPIO.BCM)
pipes = [[ 0xf0, 0xf0 ,0xf0,0xf0,0xe1 ],
	[ 0xf0, 0xf0 ,0xf0,0xf0,0xd2 ] ]

radio = NRF24(GPIO,spidev.SpiDev())
radio.begin(0,25)
radio.setPayloadSize(32)
radio.setChannel(0x4c)
radio.setDataRate(NRF24.BR_1MBPS)
radio.setPALevel(NRF24.PA_MAX)

#radio.setAutoAck(True)
#radio.enableDynamicPayloads()
radio.enableAckPayload()

radio.openWritingPipe(pipes[0])
radio.openReadingPipe(1, pipes[1])
radio.printDetails()

buf =  list(sys.argv[1])
buf =  [int(sys.argv[1])]

radio.stopListening()
radio.write(buf)
print "Sent : " , buf
radio.startListening()

