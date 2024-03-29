from machine import Pin, Timer, PWM
import time

stateDict = {
    "base":1,
    "running":2
    }

# prepare Pins
ledReady  = Pin(11, Pin.OUT)
ledWork = Pin(14, Pin.OUT)
btn  = Pin(28, Pin.IN)  #GP28
servoArm = PWM(Pin(2))
servoArm.freq(50)
servoHood = PWM(Pin(4))
servoHood.freq(50)

pwmmin = 2300 # 3276
pwmmax = 7700 # 6553
pwmstep = 50
sleeptime = 0.025

class UMState:
    startedTime = 0
    
    def __reset(self):
        self.state = stateDict["base"]
        self.startedTime = 0
        
    def __init__(self):
        self.__reset()

    def getState(self):
        return self.state

    def startRunning(self):
        if self.state != stateDict["base"]:
            return -1
        print("Start cycle")
        self.state = stateDict["running"]
        ledWork.value(1)
        self.startedTime = time.time()
        self.doCycle()
        return 1

    def stopRunning(self):
        self.__reset()
        ledWork.value(0)

    def doPrint(self):
        print("State: ", self.state, " tick: ", self.startedTime)

    def startApp(self):
        ledReady.value(1) # switch on at program start
        ledWork.value(0)
        servoHood.duty_u16(pwmmin)
        servoArm.duty_u16(pwmmin)
        time.sleep(1)

    def stopApp(self):
        ledReady.value(0)
        ledWork.value(0)
        servoHood.duty_u16(pwmmin)
        servoArm.duty_u16(pwmmin)
        time.sleep(1)
        servoHood.deinit()
        servoArm.deinit()

    def doCycle(self):
        # step1: open hood
        dc = pwmmin
        pwmdelta = pwmstep
        while dc < pwmmax:
            servoHood.duty_u16(dc)
            dc = dc + pwmdelta;
            time.sleep(sleeptime)
        servoHood.duty_u16(pwmmax) # put on max position
        
        # step 2: move arm forward and back
        dc = pwmmin
        pwmdelta = pwmstep
        while dc >= pwmmin:
            if (dc > pwmmax):
                pwmdelta = -1 * pwmstep
                dc = pwmmax
            servoArm.duty_u16(dc)
            dc = dc + pwmdelta;
            time.sleep(sleeptime)
        # step 3: close hood
        dc = pwmmax
        pwmdelta = -1 * pwmstep
        while dc > pwmmin:
            servoHood.duty_u16(dc)
            dc = dc + pwmdelta;
            time.sleep(sleeptime)
        # and back to base state
        self.stopRunning()

# -------------------- main -------------------------
um = UMState()
um.startApp()

try:
    print("Ready - waiting")
    while True:
        if (btn.value() == 1) and (um.getState() == stateDict["base"]):
            um.startRunning()
        time.sleep(0.5)  # no need for full power polling

except KeyboardInterrupt:
    print("Interrupted ctrl-c")
    #switch all off
    um.stopApp()

#the end


    

