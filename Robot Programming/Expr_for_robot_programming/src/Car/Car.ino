#define TRIG 2
#define ECHO 3
#define WHEEL_RIGHT_1 5
#define WHEEL_RIGHT_2 6
#define WHEEL_LEFT_1 9
#define WHEEL_LEFT_2 10
#define INFRARED_1 11
#define INFRARED_2 12
#define FORWARD_LIMIT 10.0
#define EPS 1e-6

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
    digitalWrite(WHEEL_LEFT_1, LOW);
    digitalWrite(WHEEL_LEFT_2, HIGH);
    digitalWrite(WHEEL_RIGHT_1, LOW);
    digitalWrite(WHEEL_RIGHT_2, HIGH);
}

void clearWheelState() {
    digitalWrite(WHEEL_LEFT_1, LOW);
    digitalWrite(WHEEL_LEFT_2, LOW);
    digitalWrite(WHEEL_RIGHT_1, LOW);
    digitalWrite(WHEEL_RIGHT_2, LOW);
}

void moveBackward() {
    digitalWrite(WHEEL_LEFT_1, HIGH);
    digitalWrite(WHEEL_LEFT_2, LOW);
    digitalWrite(WHEEL_RIGHT_1, HIGH);
    digitalWrite(WHEEL_RIGHT_2, LOW);
}

void turnRight() {
    clearWheelState();
    digitalWrite(WHEEL_RIGHT_1, HIGH);
    digitalWrite(WHEEL_RIGHT_2, LOW);
    delay(200);
    clearWheelState();
    digitalWrite(WHEEL_LEFT_1, LOW);
    digitalWrite(WHEEL_LEFT_2, HIGH);
    delay(500);
    clearWheelState();
    moveForward();
    delay(200);
}

void turnLeft() {
    clearWheelState();
    digitalWrite(WHEEL_LEFT_1, HIGH);
    digitalWrite(WHEEL_LEFT_2, LOW);
    delay(200);
    clearWheelState();
    digitalWrite(WHEEL_RIGHT_1, LOW);
    digitalWrite(WHEEL_RIGHT_2, HIGH);
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
    if (cm - FORWARD_LIMIT < EPS) {
        return 1;
    } else {
        return 0;
    }
}

int frontBlocked() {
    int right_1 = digitalRead(INFRARED_1);
    int right_2 = digitalRead(INFRARED_2);
    return !right_1 || !right_2;
}

void loop() {
    // car state init
    stateInit();

    // judge turn right
    if (!rightBlocked()) {
        turnRight();
    } else {
        // judge go straight
        if (!frontBlocked()) {
            moveForward();
            delay(100);
        } else {
            // turn left
            turnLeft();
        }
    }
}
