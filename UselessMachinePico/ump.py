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
        self.state = stateDict["running"]
        self.startedTime = time.time()
        return 1

    def stopRunning(self):
        self.__reset()

    def doPrint(self):
        print("State: ", self.state, " tick: ", self.startedTime)

	def startApp(self):
		ledReady.value(1) # switch on on program start
		letWork.value(0)
	
	def stopApp(self):
		ledReady.value(0)
		ledWork.value(0)
		servoHood.deinit()
		servoArm.deinit()


pwmmin = 2200 # 3276
pwmmax = 7900 # 6553
pwmstep = 200
pwmdelta = pwmstep
dc = pwmmin;

um = UMState()
um.startApp()

count = 0
while count < 200:
        if btn.value():
            ledWork.value(1)
        else:
            ledWork.value(0)
        servoHood.duty_u16(dc)
        # print("dc = " + str(dc))
        dc = dc + pwmdelta;
        if (dc > pwmmax):
            pwmdelta = -1 * pwmstep
            dc = pwmmax
            print("umdreh max", dc)
        if (dc < pwmmin):
            pwmdelta = pwmstep
            dc = pwmmin
            print("umdreh min", dc)
        count = count + 1;
        time.sleep(0.25)

#switch all off
um.stopApp()



    
