from machine import Pin, Timer, PWM
import time

led2 = Pin(25, Pin.OUT)  #internal LED
led  = Pin(14, Pin.OUT) #GP14
btn  = Pin(28, Pin.IN)

led2.value(1)


servo = PWM(Pin(4))
servo.freq(50)
print("Servo = ")
print(servo)

pwmmin = 2200 # 3276
pwmmax = 7900 # 6553
pwmstep = 200
pwmdelta = pwmstep

dc = pwmmin;
# timer = Timer()
# 
# def blink(timer):
#     led.toggle()
#     led2.toggle()
# 
# timer.init(freq=1, mode=Timer.PERIODIC, callback=blink)
#

count = 0
while count < 100:
        if btn.value():
            # print("Btn 1")
            led.value(1)
        else:
            # print("Btn 0")
            led.value(0)
        servo.duty_u16(dc)
        print("dc = " + str(dc))
        time.sleep(0.25)
        dc = dc + pwmdelta;
        if (dc > pwmmax):
            pwmdelta = -1 * pwmstep
            dc = pwmmax
            print("umdreh max")
        if (dc < pwmmin):
            pwmdelta = pwmstep
            dc = pwmmin
            print("umdreh min")
        count = count + 1;


led.value(0)
led2.value(0)
servo.duty_u16(pwmmin)


    
