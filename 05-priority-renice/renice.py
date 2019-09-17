import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.ADC as ADC
import time
import os

l0 = "P8_8"
l1 = "P8_10"
pot0 = "P9_36"
pot1 = "P9_38"

GPIO.setup(l0, GPIO.OUT)
GPIO.setup(l1, GPIO.OUT)

ADC.setup() 

def carga(k):
    f = 0.999999
    for i in range(0, k):
        f = f*f*f*f*f
        f = 1.56
        for j in range(0, k):
            f = f*f*f*f*f*f



def renice():
    processID1 = os.fork()
    if processID1 != 0:
        processID2 = os.fork()
        if processID2 != 0:
            while 1:
                priority0 = ADC.read(pot0)*37-17
                os.system('sudo renice %d %d'%(priority0,processID1))
                priority1 = ADC.read(pot1)*37-17
                os.system('sudo renice %d %d'%(priority1,processID2))
                time.sleep(1)
        else:
            while 1:
                carga(300)
                GPIO.output(l1, GPIO.HIGH)
                carga(300)
                GPIO.output(l1, GPIO.LOW)

    else:
        while 1:
            carga(300)
            GPIO.output(l0, GPIO.HIGH)
            carga(300)
            GPIO.output(l0, GPIO.LOW)


renice()