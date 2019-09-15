import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.ADC as ADC
import math
import time

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
            f = math.sin(f)*math.sin(f)*f*f*f

def process(led):
    while 1:
        carga(250)
        GPIO.output(led, GPIO.HIGH)
        carga(250)
        GPIO.output(led, GPIO.LOW)

def main():
    processID1 = os.fork()
    if processid1 != 0:
        processID2 = os.fork()
        if pid2 != 0:
            while 1:
                pot0 = ADC.read(ADC1)*35-15
                os.system('sudo renice %d %d'%(pot0,pid1))
                pot1 = ADC.read(ADC2)*35-15
                os.system('sudo renice %d %d'%(pot1,pid2))
                os.system('clear')
        else:
            process(l1)

    else:
        process(l0)