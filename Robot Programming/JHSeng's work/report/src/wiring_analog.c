#include "wiring_private.h"
#include "pins_arduino.h"
uint8_t analog_reference = DEFAULT;
void analogReference(uint8_t mode) {
    // 实际上无法在此处设置寄存器，因为默认设置将连接AVCC和AREF引脚，如果AREF连接了某些引脚，则会导致短路。
    analog_reference = mode;
}
int analogRead(uint8_t pin) {
    uint8_t low, high;
#if defined(analogPinToChannel)
#if defined(__AVR_ATmega32U4__)
    if (pin >= 18) pin -= 18; // 允许输入通道号或引脚号
#endif
    pin = analogPinToChannel(pin);
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    if (pin >= 54) pin -= 54; // 允许输入通道号或引脚号
#elif defined(__AVR_ATmega32U4__)
    if (pin >= 18) pin -= 18; // 允许输入通道号或引脚号
#elif defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)
    if (pin >= 24) pin -= 24; // 允许输入通道号或引脚号
#else
    if (pin >= 14) pin -= 14; // 允许输入通道号或引脚号
#endif
#if defined(ADCSRB) && defined(MUX5)
    // ADCSRB的MUX5位选择是否从通道读取
    // 0 to 7 (MUX5 low) or 8 to 15 (MUX5 high).
    ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((pin >> 3) & 0x01) << MUX5);
#endif
    // 设置模拟参考（ADMUX的高两位）并选择通道（低4位）。 这也会将ADLAR（左调整结果）设置为0（默认值）。
#if defined(ADMUX)
#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = (analog_reference << 4) | (pin & 0x07);
#else
    ADMUX = (analog_reference << 6) | (pin & 0x07);
#endif
#endif
#if defined(ADCSRA) && defined(ADCL)
    // 开始转换
    sbi(ADCSRA, ADSC);
    // 转换完成后清除ADSC
    while (bit_is_set(ADCSRA, ADSC));
    // 我们必须先阅读ADCL； 这样做会同时锁定ADCL和ADCH，直到读取ADCH。
    // 秒读取ADCL将导致每次转换的结果被丢弃，因为ADCL和ADCH完成时将被锁定。
    low  = ADCL;
    high = ADCH;
#else
    // 没有ADC，返回0
    low  = 0;
    high = 0;
#endif
    // 合成两字节
    return (high << 8) | low;
}
// 现在，PWM输出仅在具有硬件支持的引脚上起作用。 这些在相应的pins _ *。c文件中定义。
// 对于其余的引脚，我们默认为数字输出。
void analogWrite(uint8_t pin, int val) {
    // 我们需要确保为支持PWM的那些引脚启用了PWM输出，因为在对其进行数字读取或写入时将其关闭。 另外，请确保该引脚处于输出模式以与“接线”保持一致，该布线不需要为模拟输出引脚调用pinMode。
    pinMode(pin, OUTPUT);
    if (val == 0) {
        digitalWrite(pin, LOW);
    } else if (val == 255) {
        digitalWrite(pin, HIGH);
    } else {
        switch (digitalPinToTimer(pin)) {
#if defined(TCCR0) && defined(COM00) && !defined(__AVR_ATmega8__)
            case TIMER0A:
                sbi(TCCR0, COM00);
                OCR0 = val;
                break;
#endif

#if defined(TCCR0A) && defined(COM0A1)
            case TIMER0A:
                sbi(TCCR0A, COM0A1);
                OCR0A = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR0A) && defined(COM0B1)
            case TIMER0B:
                sbi(TCCR0A, COM0B1);
                OCR0B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR1A) && defined(COM1A1)
            case TIMER1A:
                sbi(TCCR1A, COM1A1);
                OCR1A = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR1A) && defined(COM1B1)
            case TIMER1B:
                sbi(TCCR1A, COM1B1);
                OCR1B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR1A) && defined(COM1C1)
            case TIMER1C:
                sbi(TCCR1A, COM1C1);
                OCR1C = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR2) && defined(COM21)
            case TIMER2:
                sbi(TCCR2, COM21);
                OCR2 = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR2A) && defined(COM2A1)
            case TIMER2A:
                sbi(TCCR2A, COM2A1);
                OCR2A = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR2A) && defined(COM2B1)
            case TIMER2B:
                sbi(TCCR2A, COM2B1);
                OCR2B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR3A) && defined(COM3A1)
            case TIMER3A:
                sbi(TCCR3A, COM3A1);
                OCR3A = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR3A) && defined(COM3B1)
            case TIMER3B:
                sbi(TCCR3A, COM3B1);
                OCR3B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR3A) && defined(COM3C1)
            case TIMER3C:
                sbi(TCCR3A, COM3C1);
                OCR3C = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR4A)
            case TIMER4A:
                sbi(TCCR4A, COM4A1);
#if defined(COM4A0)     // only used on 32U4
                cbi(TCCR4A, COM4A0);
#endif
                OCR4A = val;    // 设定pwm值班
                break;
#endif

#if defined(TCCR4A) && defined(COM4B1)
            case TIMER4B:
                sbi(TCCR4A, COM4B1);
                OCR4B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR4A) && defined(COM4C1)
            case TIMER4C:
                sbi(TCCR4A, COM4C1);
                OCR4C = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR4C) && defined(COM4D1)
            case TIMER4D:
                sbi(TCCR4C, COM4D1);
#if defined(COM4D0)     // only used on 32U4
                cbi(TCCR4C, COM4D0);
#endif
                OCR4D = val;    // 设定pwm值班
                break;
#endif


#if defined(TCCR5A) && defined(COM5A1)
            case TIMER5A:
                sbi(TCCR5A, COM5A1);
                OCR5A = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR5A) && defined(COM5B1)
            case TIMER5B:
                sbi(TCCR5A, COM5B1);
                OCR5B = val; // 设定pwm值班
                break;
#endif

#if defined(TCCR5A) && defined(COM5C1)
            case TIMER5C:
                sbi(TCCR5A, COM5C1);
                OCR5C = val; // 设定pwm值班
                break;
#endif

            case NOT_ON_TIMER:
            default:
                if (val < 128) {
                    digitalWrite(pin, LOW);
                } else {
                    digitalWrite(pin, HIGH);
                }
        }
    }
}

