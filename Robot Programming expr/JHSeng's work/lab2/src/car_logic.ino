#define TRIG 2
#define ECHO 3
#define WHEEL_RIGHT_1 5
#define WHEEL_RIGHT_2 6
#define WHEEL_LEFT_1 9
#define WHEEL_LEFT_2 10
#define INFRARED_1 11
#define INFRARED_2 12
#define FORWARD_LIMIT 50
#define EPS 1e-6
#define SPEED 120

void setup() {
    // serial init
    Serial.begin(115200);

    // built-in led init
    pinMode(LED_BUILTIN, OUTPUT);

    // ultrasonic wave init
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // wheel init
    pinMode(WHEEL_LEFT_1, OUTPUT);
    pinMode(WHEEL_LEFT_2, OUTPUT);
    pinMode(WHEEL_RIGHT_1, OUTPUT);
    pinMode(WHEEL_RIGHT_2, OUTPUT);

    // rightNotBlocked init
    pinMode(INFRARED_1, INPUT);
    pinMode(INFRARED_2, INPUT);
}

void stateInit() {
    clearWheelState();
}

void moveForward() {
    analogWrite(WHEEL_LEFT_1, 0);
    analogWrite(WHEEL_LEFT_2, SPEED);
    analogWrite(WHEEL_RIGHT_1, 0);
    analogWrite(WHEEL_RIGHT_2, SPEED);
}

void clearWheelState() {
    analogWrite(WHEEL_LEFT_1, 0);
    analogWrite(WHEEL_LEFT_2, 0);
    analogWrite(WHEEL_RIGHT_1, 0);
    analogWrite(WHEEL_RIGHT_2, 0);
}

void moveBackward() {
    analogWrite(WHEEL_LEFT_1, SPEED);
    analogWrite(WHEEL_LEFT_2, 0);
    analogWrite(WHEEL_RIGHT_1, SPEED);
    analogWrite(WHEEL_RIGHT_2, 0);
}

void turnRight() {
    clearWheelState();
    analogWrite(WHEEL_RIGHT_1, SPEED);
    analogWrite(WHEEL_RIGHT_2, 0);
    delay(500);
    clearWheelState();
    analogWrite(WHEEL_LEFT_1, 0);
    analogWrite(WHEEL_LEFT_2, SPEED);
    delay(500);
    clearWheelState();
    moveForward();
    delay(200);
}

void turnLeft() {
    clearWheelState();
    analogWrite(WHEEL_LEFT_1, SPEED);
    analogWrite(WHEEL_LEFT_2, 0);
    delay(500);
    clearWheelState();
    analogWrite(WHEEL_RIGHT_1, 0);
    analogWrite(WHEEL_RIGHT_2, SPEED);
    delay(500);
    clearWheelState();
    moveForward();
    delay(200);
}

int rightBlocked() {
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    float temp = float(pulseIn(ECHO, HIGH));
    float cm = temp * 17 / 1000.0;
    Serial.print("Echo = ");
    Serial.print(temp);
    Serial.print(" | Distance = ");
    Serial.print(cm);
    Serial.println("cm");
    if (cm - FORWARD_LIMIT < EPS) {
        return 1;
    } else {
        return 0;
    }
}

int frontTest() {
    int leftNotBlocked = digitalRead(INFRARED_1);
    int rightNotBlocked = digitalRead(INFRARED_2);
    if (leftNotBlocked && rightNotBlocked) return 1;
    else if (leftNotBlocked && !rightNotBlocked) return 2;
    else if (!leftNotBlocked && rightNotBlocked) return 3;
    else return 4;
}

void loop() {
    // car state init
    stateInit();
    int res = frontTest();
    if (res == 1) {
        moveForward();
        delay(300);
    } else if (res == 2 || res == 3) {
        if (!rightBlocked()) turnRight();
        else turnLeft();
    } else if (!rightBlocked()) {
        turnRight();
    } else turnLeft();
}