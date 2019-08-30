import Adafruit_BBIO.GPIO as GPIO
import Adafruit_BBIO.ADC as ADC
import Adafruit_BBIO.PWM as PWM 
import time
import random

#Para o buzzer

#GPIO's botoes
b0 = "P8_7"
b1 = "P8_8"
b2 = "P8_9"
b3 = "P8_10"

#GPIO's leds
l0 = "P8_13"
l1 = "P8_14"
l2 = "P8_15"
l3 = "P8_16"

#GPIO's buzzer
buzzer = "P9_14"

#GPIO's display
d11 = "P9_11"
d12 = "P9_12"
d13 = "P9_13"
d15 = "P9_15"
d16 = "P9_16"
d17 = "P9_17"
d18 = "P9_18"

#LED VECTOR
LEDS = [l0,l1,l2,l3]

game_sequence = []
player_sequence = []

#Definindo entradas e saidas
GPIO.setup(b0, GPIO.IN)
GPIO.setup(b1, GPIO.IN)
GPIO.setup(b2, GPIO.IN)
GPIO.setup(b3, GPIO.IN)
GPIO.setup(l0, GPIO.OUT)
GPIO.setup(l1, GPIO.OUT)
GPIO.setup(l2, GPIO.OUT)
GPIO.setup(l3, GPIO.OUT)
GPIO.setup(buzzer, GPIO.OUT)
GPIO.setup(d11, GPIO.OUT)
GPIO.setup(d12, GPIO.OUT)
GPIO.setup(d13, GPIO.OUT)
GPIO.setup(d15, GPIO.OUT)
GPIO.setup(d16, GPIO.OUT)
GPIO.setup(d17, GPIO.OUT)
GPIO.setup(d18, GPIO.OUT)

GPIO.add_event_detect(b0, GPIO.FALLING)
GPIO.add_event_detect(b1, GPIO.FALLING)
GPIO.add_event_detect(b2, GPIO.FALLING)
GPIO.add_event_detect(b3, GPIO.FALLING)

current_round = 1
game_started = False
game_over = False

def display():
	if((current_round%10) == 1):
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
	if((current_round%10) == 2):
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d16, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
	if((current_round%10) == 3):
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
	if((current_round%10) == 4):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
	if((current_round%10) == 5):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
	if((current_round%10) == 6):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d16, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
	if((current_round%10) == 7):
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
	if((current_round%10) == 8):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d16, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
		GPIO.output(d17, GPIO.HIGH)
	if((current_round%10) == 9):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d12, GPIO.HIGH)
		GPIO.output(d17, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)	
	if((current_round%10) == 0):
		GPIO.output(d11, GPIO.HIGH)
		GPIO.output(d18, GPIO.HIGH)
		GPIO.output(d16, GPIO.HIGH)
		GPIO.output(d15, GPIO.HIGH)
		GPIO.output(d13, GPIO.HIGH)
		GPIO.output(d17, GPIO.HIGH)	

def displayDown():
	GPIO.output(d17, GPIO.LOW)
	GPIO.output(d18, GPIO.LOW)
	GPIO.output(d12, GPIO.LOW)
	GPIO.output(d16, GPIO.LOW)
	GPIO.output(d11, GPIO.LOW)
	GPIO.output(d13, GPIO.LOW)
	GPIO.output(d15, GPIO.LOW)

def blinkAll(tempo):
    GPIO.output(l0, GPIO.HIGH)
    time.sleep(tempo)  
    GPIO.output(l3, GPIO.HIGH)
    time.sleep(tempo) 
    GPIO.output(l1, GPIO.HIGH)
    time.sleep(tempo) 
    GPIO.output(l2, GPIO.HIGH)
    time.sleep(tempo)
    GPIO.output(l0, GPIO.LOW)
    GPIO.output(l3, GPIO.LOW)
    GPIO.output(l1, GPIO.LOW)
    GPIO.output(l2, GPIO.LOW)
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
	global game_started 
	global game_over
	global current_round
	current_round = 1
	game_started = False
	del player_sequence[:]
	del game_sequence[:]
	time.sleep(0.3) #Delay
	blinkAll(0.1) 
	game_over = True
	displayDown()
	#print("game_over*gameOver: ")
	#print(game_over)

def generate_current_round():
    #A cada round cont aumenta
    current_led = random.randint(0,3)
    game_sequence.append(current_led)
    print("game_sequence:")
    print(game_sequence)
    print("current_round:")
    print(current_round)
    for count in range(0,current_round):
        blink(LEDS[game_sequence[count]],0.5)  


def get_play():
    if(current_round > 1):
        del player_sequence[:]
    number_of_plays = 0
    time_begin = time.time() 
# Retorna o tempo em segundos
    time_end = time.time() 
    # O jogador tem 1 segundo para fazer a sequencia
    while((time_end - time_begin) < current_round + 2):

        if(GPIO.input(b0)):
            player_sequence.append(0)
            number_of_plays += 1
	    PWM.start(buzzer, 50, 262, 1) #DO
	    time.sleep(0.3)
	    PWM.stop(buzzer)
            print("B0")
            time.sleep(0.25)

        if(GPIO.input(b1)):
            player_sequence.append(1)
            number_of_plays += 1
            PWM.start(buzzer, 50, 294, 1) #RE
	    time.sleep(0.3)
	    PWM.stop(buzzer)
            print("B1")
            time.sleep(0.25)
	if(GPIO.input(b2)):
            player_sequence.append(2)
            number_of_plays += 1
            PWM.start(buzzer, 50, 330, 1) #MI
	    time.sleep(0.3)
	    PWM.stop(buzzer)
            print("B2")
            time.sleep(0.25)

        if(GPIO.input(b3)):
            player_sequence.append(3)
            number_of_plays += 1
	    PWM.start(buzzer, 50, 300, 1) #FA
	    time.sleep(0.3)
	    PWM.stop(buzzer)
            print("B3")
            time.sleep(0.25)

        time_end = time.time()
        if(number_of_plays == current_round):
            break
    if(number_of_plays < current_round):
	blinkAll(0.1)
	PWM.start(buzzer, 50, 2000, 1) #DO
	time.sleep(1)
	PWM.stop(buzzer) 
	gameOver()
	print("GAME OVER - TEMPO ESGOTADO")

def validate_current_round():
    global game_over
    if(game_over):
	return(False)
    else:
	print("current_round*validate...:")
	print(current_round)
	for i in range(0,current_round):
	        if(player_sequence[i] != game_sequence[i]):
	            return(False)
    return(True)

def delay(j):  #need to overwrite delay() otherwise, it's too slow
    for k in range(1,j):
            pass

while True:
    #Aperte qualquer botao para iniciar
    if(GPIO.input(b0)):
	game_started = True
	current_round = 1
	game_over = False
    while(game_started):

	    display()

	    generate_current_round()    

	    #Pega a sequencia feita pelo jogador
	    get_play()

	    print(game_sequence)
	    print(player_sequence)
	    #print("game_over*MAIN:")
	    #print(game_over)

	    if(game_over):
		#print("if(game_over)")
		#print(current_round)
		game_started = False
		break
	    elif(not(validate_current_round())):
		blinkAll(0.1)
		PWM.start(buzzer, 50, 2000, 1) #DO
		time.sleep(1)
		PWM.stop(buzzer) 
		print("GAME OVER")
		current_round = 1
		gameOver()
		break
	    
	    displayDown()
	    current_round += 1



