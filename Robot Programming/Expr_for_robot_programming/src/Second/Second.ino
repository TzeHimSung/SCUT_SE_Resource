#define Trig 2
#define Echo 3

float cm, temp;

void setup() {
    // put your setup code here, to run once:

    // LED测试
    //  pinMode(LED_BUILTIN, OUTPUT);

    // 红外测试
    //  pinMode(2, INPUT);
    //  pinMode(LED_BUILTIN, OUTPUT);

    // 超声波测试
    Serial.begin(115200);
    pinMode(Trig, OUTPUT);
    pinMode(Echo, INPUT);
}

void loop() {
    // put your main code here, to run repeatedly:

    // LED测试
    //  digitalWrite(LED_BUILTIN, HIGH);

    // 串口测试
    //  SoftwareSerial mySerial(2,3);
    //  if (mySerial.available())
    //    Serial.write(mySerial.read());
    //  if (Serial.available())
    //    Serial.write(Serial.read());

    // 红外测试
    //  digitalWrite(LED_BUILTIN, digitalRead(2));

    // 超声波测试
    //  digitalWrite(Trig, LOW);
    //  delay(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);

    temp = float(pulseIn(Echo, HIGH));
    cm = temp * 17.0 / 1000.0;

    Serial.print("Echo = ");
    Serial.print(temp);
    Serial.print(" | Distance = ");
    Serial.print(cm);
    Serial.println("cm");
    delay(100);
    //  Serial.println("test");
}