import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.ADC as ADC
import Adafruit_BBIO.PWM as PWM 
import time
import random

#GPIO's
button0 = "P8_7"
button1 = "P8_8"
button2 = "P8_9"
button3 = "P8_10"
led0 = "P8_13"
led1 = "P8_14"
led2 = "P8_15"
led3 = "P8_16"
buzzer = "P9_14"

#LED VECTOR
LEDS = [led0,led1,led2,led3]

game_sequence = []
player_sequence = []

#Definindo entradas e saidas
GPIO.setup(button0, GPIO.IN)
GPIO.setup(button1, GPIO.IN)
GPIO.setup(button2, GPIO.IN)
GPIO.setup(button3, GPIO.IN)
GPIO.setup(led0, GPIO.OUT)
GPIO.setup(led1, GPIO.OUT)
GPIO.setup(led2, GPIO.OUT)
GPIO.setup(led3, GPIO.OUT)
GPIO.setup(buzzer, GPIO.OUT)

GPIO.add_event_detect(button0, GPIO.FALLING)
GPIO.add_event_detect(button1, GPIO.FALLING)
GPIO.add_event_detect(button2, GPIO.FALLING)
GPIO.add_event_detect(button3, GPIO.FALLING)

current_round = 1
game_started = False


def blinkAll(tempo):
    GPIO.output(led0, GPIO.HIGH)
    time.sleep(tempo)  
    GPIO.output(led3, GPIO.HIGH)
    time.sleep(tempo) 
    GPIO.output(led1, GPIO.HIGH)
    time.sleep(tempo) 
    GPIO.output(led2, GPIO.HIGH)
    time.sleep(tempo)
    GPIO.output(led0, GPIO.LOW)
    GPIO.output(led3, GPIO.LOW)
    GPIO.output(led1, GPIO.LOW)
    GPIO.output(led2, GPIO.LOW)
    time.sleep(tempo)

def blink(led,tempo):
    GPIO.output(led, GPIO.HIGH)
    if(led == "P8_13"):
        PWM.start(buzzer, 50, 262, 1) #DO
    time.sleep(0.5)
    PWM.stop(buzzer)
    if(led == "P8_14"):
        PWM.start(buzzer, 50, 294, 1) #RE
    time.sleep(0.5)
    PWM.stop(buzzer)
    if(led == "P8_15"):
        PWM.start(buzzer, 50, 330, 1) #MI
    time.sleep(0.5)
    PWM.stop(buzzer)
    if(led == "P8_16"):
        PWM.start(buzzer, 50, 300, 1) #FA
    time.sleep(0.5)
    PWM.stop(buzzer)

    GPIO.output(led, GPIO.LOW)
    time.sleep(tempo)

    #Game Over
    def gameOver(): 
        time.sleep(0.3) #Delay
        blinkAll(0.1) 

    def generate_current_round():
        #A cada rount cont aumenta
        current_led = random.randint(0,3)
        game_sequence.append(current_led)
        for count in range(0,current_round):
            blink(LEDS[game_sequence[count]],0.5)  


    def play():
        if(current_round > 1):
            del player_sequence[:]
        number_of_plays = 0
        time_begin = time.time() 
        # Retorna o tempo em segundos
        time_end = time.time() 
        # O jogador tem // segundo para fazer a sequencia
        while((time_end - time_begin) < current_round + 1):

            if(GPIO.input(button0)):
                player_sequence.append(0)
                number_of_plays += 1
                PWM.start(buzzer, 50, 262, 1) #DO
                time.sleep(0.3)
                PWM.stop(buzzer)
                print("button0")
                time.sleep(0.25)

            if(GPIO.input(button1)):
                player_sequence.append(1)
                number_of_plays += 1
                PWM.start(buzzer, 50, 294, 1) #RE
                time.sleep(0.3)
                PWM.stop(buzzer)
                print("button1")
                time.sleep(0.25)
            if(GPIO.input(button2)):
                player_sequence.append(2)
                number_of_plays += 1
                PWM.start(buzzer, 50, 330, 1) #MI
                time.sleep(0.3)
                PWM.stop(buzzer)
                print("button2")
                time.sleep(0.25)

            if(GPIO.input(button3)):
                player_sequence.append(3)
                number_of_plays += 1
                PWM.start(buzzer, 50, 300, 1) #FA
                time.sleep(0.3)
                PWM.stop(buzzer)
                print("button3")
                time.sleep(0.25)

            time_end = time.time()
            if(number_of_plays == current_round):
                break
        if(number_of_plays < current_round):
            blinkAll(0.1)
            PWM.start(buzzer, 50, 262, 1) #DO
            time.sleep(1)
            PWM.stop(buzzer) 
            print("GAME OVER - TEMPO ESGOTADO")
            game_started = False
            gameOver()

    def validate_current_round():
        for i in range(0,current_round):
            if(player_sequence[i] != game_sequence[i]):
                return(False)
        return(True)

    while True:
        #Aperte qualquer botao para iniciar
        while(GPIO.input(button0) or GPIO.input(button1) or GPIO.input(button2) or GPIO.input(button3)):

            game_started = True

            while game_started:

                generate_current_round()    

                #Pega a sequencia feita pelo jogador
                play()

                #TO DEBUG
                print(game_sequence)
                print(player_sequence)

                if(not(validate_current_round())):
                    blinkAll(0.1)
                    PWM.start(buzzer, 50, 262, 1) #DO
                    time.sleep(1)
                    PWM.stop(buzzer) 
                    print("GAME OVER")
                    gameOver()
                    game_started = False


                current_round += 1



