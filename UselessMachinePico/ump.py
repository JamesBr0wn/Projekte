from machine import Pin, Timer
import time

led2 = Pin(25, Pin.OUT)  #internal LED
led  = Pin(14, Pin.OUT) #GP14
btn  = Pin(28, Pin.IN)

led2.value(1)

# timer = Timer()
# 
# def blink(timer):
#     led.toggle()
#     led2.toggle()
# 
# timer.init(freq=1, mode=Timer.PERIODIC, callback=blink)
#

count = 0
while count < 41:
        if btn.value():
            # print("Btn 1")
            led.value(1)
        else:
            # print("Btn 0")
            led.value(0)
        time.sleep(0.25)
        count = count + 1;

led.value(0)
led2.value(0)


    
