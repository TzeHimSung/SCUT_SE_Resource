import wiringpi
import time

# infrared pin
INFR_LEFT = 0
INFR_RIGHT = 1
# ultra sound
ECHO = 2
TRIG = 3
# wheel pin
WHEEL_1 = 6
WHEEL_2 = 10
WHEEL_3 = 11
WHEEL_4 = 31
# wheel speed 
FAST = 90
SLOW = 50
# epsilon
EPS = 0.000001
LIM = 50
# infr initial state
left_infr = 0
right_infr = 0

# pin init
def init():
	# pin init
	wiringpi.wiringPiSetup()
	# wheel pin init
	wiringpi.pinMode(WHEEL_1,1)
	wiringpi.pinMode(WHEEL_2,1)
	wiringpi.pinMode(WHEEL_3,1)
	wiringpi.pinMode(WHEEL_4,1)
	wiringpi.softPwmCreate(WHEEL_1,0,200)
	wiringpi.softPwmCreate(WHEEL_2,0,200)
	wiringpi.softPwmCreate(WHEEL_3,0,200)
	wiringpi.softPwmCreate(WHEEL_4,0,200)
	# infrared init
	wiringpi.pinMode(INFR_LEFT,0)
	wiringpi.pinMode(INFR_RIGHT,0)
	# ultra sound init
	wiringpi.pinMode(TRIG,1)
	wiringpi.pinMode(ECHO,0)
	wiringpi.digitalWrite(TRIG,0)

# car stop
def stop():
	wiringpi.softPwmWrite(WHEEL_1,0)
	wiringpi.softPwmWrite(WHEEL_2,0)
	wiringpi.softPwmWrite(WHEEL_3,0)
	wiringpi.softPwmWrite(WHEEL_4,0)

# car go forward
def goForward():
	wiringpi.softPwmWrite(WHEEL_1,FAST)
	wiringpi.softPwmWrite(WHEEL_2,0)
	wiringpi.softPwmWrite(WHEEL_3,0)
	wiringpi.softPwmWrite(WHEEL_4,FAST)

# car go backward
def goBackward():
	wiringpi.softPwmWrite(WHEEL_1,0)
	wiringpi.softPwmWrite(WHEEL_2,FAST)
	wiringpi.softPwmWrite(WHEEL_3,FAST)
	wiringpi.softPwmWrite(WHEEL_4,0)

# car turn right
def turnRight():
	stop()
	wiringpi.softPwmWrite(WHEEL_1,0)
	wiringpi.softPwmWrite(WHEEL_2,FAST)
	wiringpi.softPwmWrite(WHEEL_3,0)
	wiringpi.softPwmWrite(WHEEL_4,SLOW)
	wiringpi.delay(200)
	stop()

# car turn left
def turnLeft():
	stop()
	wiringpi.softPwmWrite(WHEEL_1,FAST)
	wiringpi.softPwmWrite(WHEEL_2,0)
	wiringpi.softPwmWrite(WHEEL_3,SLOW)
	wiringpi.softPwmWrite(WHEEL_4,0)
	wiringpi.delay(200)
	stop()

# car infr check
def infrCheck():
	left_infr = wiringpi.digitalRead(INFR_LEFT)
	right_infr = wiringpi.digitalRead(INFR_RIGHT)
	if left_infr == 0 and right_infr == 0:
		return 0
	elif left_infr == 1 and right_infr == 0:
		return 1
	elif left_infr == 0 and right_infr == 1:
		return 2
	else:
		return 3

def ultraCheck():
	wiringpi.digitalWrite(TRIG,1)
	time.sleep(1e-5)
	wiringpi.digitalWrite(TRIG,0)
	while wiringpi.digitalRead(ECHO) == 0:
		pass
	t1 = time.time()
	while wiringpi.digitalRead(ECHO) == 1:
		pass
	t2 = time.time()
	deltaTime = (t2-t1) * 1000000
	distance = deltaTime * 17 / 1000.0
	print('Current distance =',distance,'cm')
	if distance - LIM < EPS:
		return 0
	else:
		return 1

def main():
	stop()
	frontState = infrCheck()
	if frontState == 0:
		print('Current left == 0, right == 0')
		rightState = ultraCheck()
		if rightState == 1:
			turnRight()
		else:
			turnLeft()
	elif frontState == 1:
		print('Current left == 1, right == 0')
		turnLeft()
	elif frontState == 2:
		print('Current left == 0, right == 1')
		turnRight()
	else:
		print('Current left == 1, right == 1')
		goForward()
		wiringpi.delay(200)

if __name__=='__main__':
	init()
	while (True):
		main()
