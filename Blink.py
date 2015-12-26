import RPi.GPIO as GPIO  
import time  
import sys	
# blinking function  

relayPin = int(sys.argv[1])
def blink(pin):  
        GPIO.output(pin,GPIO.HIGH)  
        time.sleep(0.5)  
        GPIO.output(pin,GPIO.LOW)  
        return  
# to use Raspberry Pi board pin numbers  
GPIO.setmode(GPIO.BOARD)  
# set up GPIO output channel  
GPIO.setup(relayPin, GPIO.OUT)  
blink(relayPin)  
GPIO.cleanup() 
