from machine import Pin, Timer, PWM
import time

# led2 = Pin(25, Pin.OUT) #internal LED
led  = Pin(14, Pin.OUT) #GP14
led2 = Pin(11, Pin.OUT)
btn  = Pin(22, Pin.IN)

led2.value(1) # switch on on program start

# prepare servo
servo = PWM(Pin(9))
servo.freq(50)


pwmmiddle = 4000
dc = 0
servo.duty_u16(pwmmiddle)
            
count = 0
while count < 2000:
        if btn.value():
            led.value(1)
            dc = pwmmiddle
        else:
            led.value(0)
            dc = pwmmiddle+1000
        servo.duty_u16(dc)
        print("dc = " + str(dc))
        count = count + 1
        time.sleep(0.5)

		#switch all off
led.value(0)
led2.value(0)
servo.deinit()



    
