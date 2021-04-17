from machine import Pin, Timer, PWM
import time

stateDict = {
    "base":1,
    "running":2
    }

# prepare Pins
ledReady  = Pin(11, Pin.OUT)
ledWork = Pin(14, Pin.OUT)
btn  = Pin(22, Pin.IN)
servoArm = PWM(Pin(9))
servoArm.freq(50)
servoHood = PWM(Pin(10))
servoHood.freq(50)

pwmmin = 2300 # 3276
pwmmax = 7700 # 6553
pwmstep = 50
sleeptime = 0.025

class UMState:
    cycleStartedTime = 0
    appStartedTime = 0
    
    def __reset(self):
        self.state = stateDict["base"]
        self.cycleStartedTime = 0
        
    def __init__(self):
        self.__reset()
        self.appStartedTime = time.time()

    def getState(self):
        return self.state

    def startRunning(self):
        if self.state != stateDict["base"]:
            return -1
        print("Start cycle")
        self.state = stateDict["running"]
        self.cycleStartedTime = time.time()
        self.doCycle()
        return 1

    def stopRunning(self):
        self.__reset()

    def doPrint(self):
        print("State: ", self.state, " tick: ", self.startedTime)

    def basePosPios(self):
        ledReady.value(1)
        ledWork.value(0)
        servoHood.duty_u16(pwmmin)
        servoArm.duty_u16(pwmmin)
        time.sleep(1)
        servoHood.deinit() #no need for constant pwm output
        servoArm.deinit()

    def stopPios(self):
        ledReady.value(0)
        ledWork.value(0)
        servoHood.duty_u16(pwmmin)
        servoArm.duty_u16(pwmmin)
        time.sleep(1)
        servoHood.deinit()
        servoArm.deinit()

    def doCycle(self):
        # step 0: signal working
        ledWork.value(1)
        # step1: open hood
        dc = pwmmin
        pwmdelta = pwmstep
        while dc < pwmmax:
            servoHood.duty_u16(dc)
            dc = dc + pwmdelta
            time.sleep(sleeptime)
        servoHood.duty_u16(pwmmax) # put on max position
        
        # step 2: move arm forward until it is either at max, or button switched
        dc = pwmmin
        pwmdelta = pwmstep
        while btn.value() == 1:
            if dc < pwmmax: # keep moving forward
                servoArm.duty_u16(dc)
                dc = dc + pwmdelta
            # else:
                # print("Max")
            time.sleep(sleeptime)

        # step 3: move arm back
        pwmdelta = -1 * pwmstep
        while dc >= pwmmin:
            servoArm.duty_u16(dc)
            dc = dc + pwmdelta;
            time.sleep(sleeptime)
        
        # step 4: close hood
        dc = pwmmax
        pwmdelta = -1 * pwmstep
        while dc > pwmmin:
            servoHood.duty_u16(dc)
            dc = dc + pwmdelta;
            time.sleep(sleeptime)
        # and back to base state
        self.basePosPios()
        self.stopRunning()


# -------------------- main -------------------------
ledReady.value(1) # switch on at program start
time.sleep(1) #delay all actions

um = UMState()
um.basePosPios()

try:
    print("Ready - waiting")
    while True:
        if (btn.value() == 1) and (um.getState() == stateDict["base"]):
            um.startRunning()
        time.sleep(0.5)  # no need for full power polling

except KeyboardInterrupt:
    print("Interrupted ctrl-c")
    #switch all off
    um.stopPios()

#the end


    

