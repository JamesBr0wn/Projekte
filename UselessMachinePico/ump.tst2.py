from machine import Pin, Timer, PWM
import time

pwmmin = 2300 # 3276
pwmmax = 7800 # 6553
pwmstep = 50
sleeptime = 0.025

# led2 = Pin(25, Pin.OUT) #internal LED
led  = Pin(14, Pin.OUT) #GP14
print("Led Pin14 = ", led)
led2 = Pin(11, Pin.OUT)
print("Led2 Pin11 = ", led2)
btn  = Pin(28, Pin.IN)  #GP28
print("Btn Pin28 = ", btn)

led2.value(1) # switch on at program start

# prepare servo
servo = PWM(Pin(4))
servo.freq(50)
print("Servo Pin4 = ", servo)
servo2 = PWM(Pin(2))
servo2.freq(50)
print("Servo2 Pin2 = ", servo2)

dc = pwmmin
pwmdelta = pwmstep

count = 0
while count < 2000:
    if btn.value():
        led.value(1)
    else:
        led.value(0)
    if (dc > pwmmax):
        pwmdelta = -1 * pwmstep
        dc = pwmmax
    if dc < pwmmin:
        pwmdelta = pwmstep
        dc = pwmmin
    servo.duty_u16(dc)
    servo2.duty_u16(dc)
    dc = dc + pwmdelta;
    time.sleep(sleeptime)
    count += 1

led.value(0)
led2.value(0)
servo.deinit()
servo2.deinit()
