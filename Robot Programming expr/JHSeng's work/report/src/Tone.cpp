#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "pins_arduino.h"

#if defined(__AVR_ATmega8__) || defined(__AVR_ATmega128__)
#define TCCR2A TCCR2
#define TCCR2B TCCR2
#define COM2A1 COM21
#define COM2A0 COM20
#define OCR2A OCR2
#define TIMSK2 TIMSK
#define OCIE2A OCIE2
#define TIMER2_COMPA_vect TIMER2_COMP_vect
#define TIMSK1 TIMSK
#endif

// timerx_toggle_count:
//> 0-指定的持续时间
// = 0-停止
// <0-无限（直到调用stop（）方法或调用新play（）为止）

#if !defined(__AVR_ATmega8__)
volatile long timer0_toggle_count;
volatile uint8_t *timer0_pin_port;
volatile uint8_t timer0_pin_mask;
#endif

volatile long timer1_toggle_count;
volatile uint8_t *timer1_pin_port;
volatile uint8_t timer1_pin_mask;
volatile long timer2_toggle_count;
volatile uint8_t *timer2_pin_port;
volatile uint8_t timer2_pin_mask;

#if defined(TIMSK3)
volatile long timer3_toggle_count;
volatile uint8_t *timer3_pin_port;
volatile uint8_t timer3_pin_mask;
#endif

#if defined(TIMSK4)
volatile long timer4_toggle_count;
volatile uint8_t *timer4_pin_port;
volatile uint8_t timer4_pin_mask;
#endif

#if defined(TIMSK5)
volatile long timer5_toggle_count;
volatile uint8_t *timer5_pin_port;
volatile uint8_t timer5_pin_mask;
#endif


#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define AVAILABLE_TONE_PINS 1
#define USE_TIMER2

const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2 /*, 3, 4, 5, 1, 0 */ };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 /*, 255, 255, 255, 255, 255 */ };

#elif defined(__AVR_ATmega8__)

#define AVAILABLE_TONE_PINS 1
#define USE_TIMER2

const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2 /*, 1 */ };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 /*, 255 */ };

#elif defined(__AVR_ATmega32U4__)

#define AVAILABLE_TONE_PINS 1
#define USE_TIMER3

const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 3 /*, 1 */ };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 /*, 255 */ };

#else

#define AVAILABLE_TONE_PINS 1
#define USE_TIMER2

// Leave timer 0 to last.
const uint8_t PROGMEM tone_pin_to_timer_PGM[] = { 2 /*, 1, 0 */ };
static uint8_t tone_pins[AVAILABLE_TONE_PINS] = { 255 /*, 255, 255 */ };

#endif



static int8_t toneBegin(uint8_t _pin) {
    int8_t _timer = -1;

    // 如果我们已经在使用该引脚，则应该配置计时器。
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == _pin) {
            return pgm_read_byte(tone_pin_to_timer_PGM + i);
        }
    }

    // 搜索未使用的计时器。
    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == 255) {
            tone_pins[i] = _pin;
            _timer = pgm_read_byte(tone_pin_to_timer_PGM + i);
            break;
        }
    }

    if (_timer != -1) {
        // 设置特定于计时器的内容
        // 所有计时器处于CTC模式
        // 8位定时器将需要更改预分频值，
        // 而16位定时器设置为ck / 1或ck / 64预分频器
        switch (_timer) {
#if defined(TCCR0A) && defined(TCCR0B) && defined(WGM01)
            case 0:
                // 8位定时器
                TCCR0A = 0;
                TCCR0B = 0;
                bitWrite(TCCR0A, WGM01, 1);
                bitWrite(TCCR0B, CS00, 1);
                timer0_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer0_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif

#if defined(TCCR1A) && defined(TCCR1B) && defined(WGM12)
            case 1:
                // 16位计时器
                TCCR1A = 0;
                TCCR1B = 0;
                bitWrite(TCCR1B, WGM12, 1);
                bitWrite(TCCR1B, CS10, 1);
                timer1_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer1_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif

#if defined(TCCR2A) && defined(TCCR2B)
            case 2:
                // 8位定时器
                TCCR2A = 0;
                TCCR2B = 0;
                bitWrite(TCCR2A, WGM21, 1);
                bitWrite(TCCR2B, CS20, 1);
                timer2_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer2_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif

#if defined(TCCR3A) && defined(TCCR3B) &&  defined(TIMSK3)
            case 3:
                // 16位计时器
                TCCR3A = 0;
                TCCR3B = 0;
                bitWrite(TCCR3B, WGM32, 1);
                bitWrite(TCCR3B, CS30, 1);
                timer3_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer3_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif

#if defined(TCCR4A) && defined(TCCR4B) &&  defined(TIMSK4)
            case 4:
                // 16位计时器
                TCCR4A = 0;
                TCCR4B = 0;
#if defined(WGM42)
                bitWrite(TCCR4B, WGM42, 1);
#elif defined(CS43)
                // atmega32u4
                bitWrite(TCCR4B, CS43, 1);
#endif
                bitWrite(TCCR4B, CS40, 1);
                timer4_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer4_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif

#if defined(TCCR5A) && defined(TCCR5B) &&  defined(TIMSK5)
            case 5:
                // 16位计时器
                TCCR5A = 0;
                TCCR5B = 0;
                bitWrite(TCCR5B, WGM52, 1);
                bitWrite(TCCR5B, CS50, 1);
                timer5_pin_port = portOutputRegister(digitalPinToPort(_pin));
                timer5_pin_mask = digitalPinToBitMask(_pin);
                break;
#endif
        }
    }

    return _timer;
}



// 频率（以赫兹为单位）和持续时间（以毫秒为单位）。

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration) {
    uint8_t prescalarbits = 0b001;
    long toggle_count = 0;
    uint32_t ocr = 0;
    int8_t _timer;

    _timer = toneBegin(_pin);

    if (_timer >= 0) {
        // 将pinMode设置为OUTPUT
        pinMode(_pin, OUTPUT);

        // 如果使用8位定时器, 扫描预分频器以找到最合适的位置
        if (_timer == 0 || _timer == 2) {
            ocr = F_CPU / frequency / 2 - 1;
            prescalarbits = 0b001;
            if (ocr > 255) {
                ocr = F_CPU / frequency / 2 / 8 - 1;
                prescalarbits = 0b010;

                if (_timer == 2 && ocr > 255) {
                    ocr = F_CPU / frequency / 2 / 32 - 1;
                    prescalarbits = 0b011;
                }

                if (ocr > 255) {
                    ocr = F_CPU / frequency / 2 / 64 - 1;
                    prescalarbits = _timer == 0 ? 0b011 : 0b100;

                    if (_timer == 2 && ocr > 255) {
                        ocr = F_CPU / frequency / 2 / 128 - 1;
                        prescalarbits = 0b101;
                    }

                    if (ocr > 255) {
                        ocr = F_CPU / frequency / 2 / 256 - 1;
                        prescalarbits = _timer == 0 ? 0b100 : 0b110;
                        if (ocr > 255) {
                            ocr = F_CPU / frequency / 2 / 1024 - 1;
                            prescalarbits = _timer == 0 ? 0b101 : 0b111;
                        }
                    }
                }
            }

#if defined(TCCR0B)
            if (_timer == 0) {
                TCCR0B = (TCCR0B & 0b11111000) | prescalarbits;
            } else
#endif
#if defined(TCCR2B)
            {
                TCCR2B = (TCCR2B & 0b11111000) | prescalarbits;
            }
#else
            {
                // 虚拟占位符以使上述ifdefs工作
            }
#endif
        } else {
            ocr = F_CPU / frequency / 2 - 1;

            prescalarbits = 0b001;
            if (ocr > 0xffff) {
                ocr = F_CPU / frequency / 2 / 64 - 1;
                prescalarbits = 0b011;
            }

            if (_timer == 1) {
#if defined(TCCR1B)
                TCCR1B = (TCCR1B & 0b11111000) | prescalarbits;
#endif
            }
#if defined(TCCR3B)
            else if (_timer == 3)
                TCCR3B = (TCCR3B & 0b11111000) | prescalarbits;
#endif
#if defined(TCCR4B)
            else if (_timer == 4)
                TCCR4B = (TCCR4B & 0b11111000) | prescalarbits;
#endif
#if defined(TCCR5B)
            else if (_timer == 5)
                TCCR5B = (TCCR5B & 0b11111000) | prescalarbits;
#endif
        }
        // 计算切换计数
        if (duration > 0) {
            toggle_count = 2 * frequency * duration / 1000;
        } else {
            toggle_count = -1;
        }

        // 设置给定定时器的OCR，设置触发计数，然后打开中断
        switch (_timer) {

#if defined(OCR0A) && defined(TIMSK0) && defined(OCIE0A)
            case 0:
                OCR0A = ocr;
                timer0_toggle_count = toggle_count;
                bitWrite(TIMSK0, OCIE0A, 1);
                break;
#endif

            case 1:
#if defined(OCR1A) && defined(TIMSK1) && defined(OCIE1A)
                OCR1A = ocr;
                timer1_toggle_count = toggle_count;
                bitWrite(TIMSK1, OCIE1A, 1);
#elif defined(OCR1A) && defined(TIMSK) && defined(OCIE1A)
                // 这种组合至少适用于ATmega32
                OCR1A = ocr;
                timer1_toggle_count = toggle_count;
                bitWrite(TIMSK, OCIE1A, 1);
#endif
                break;

#if defined(OCR2A) && defined(TIMSK2) && defined(OCIE2A)
            case 2:
                OCR2A = ocr;
                timer2_toggle_count = toggle_count;
                bitWrite(TIMSK2, OCIE2A, 1);
                break;
#endif

#if defined(OCR3A) && defined(TIMSK3) && defined(OCIE3A)
            case 3:
                OCR3A = ocr;
                timer3_toggle_count = toggle_count;
                bitWrite(TIMSK3, OCIE3A, 1);
                break;
#endif

#if defined(OCR4A) && defined(TIMSK4) && defined(OCIE4A)
            case 4:
                OCR4A = ocr;
                timer4_toggle_count = toggle_count;
                bitWrite(TIMSK4, OCIE4A, 1);
                break;
#endif

#if defined(OCR5A) && defined(TIMSK5) && defined(OCIE5A)
            case 5:
                OCR5A = ocr;
                timer5_toggle_count = toggle_count;
                bitWrite(TIMSK5, OCIE5A, 1);
                break;
#endif

        }
    }
}
// 此功能仅适用于计时器2（我们目前使用的唯一计时器）。 对于其他的，它应该结束音调，但是不会为计时器恢复适当的PWM功能。
void disableTimer(uint8_t _timer) {
    switch (_timer) {
        case 0:
#if defined(TIMSK0)
            TIMSK0 = 0;
#elif defined(TIMSK)
            TIMSK = 0; // atmega32
#endif
            break;

#if defined(TIMSK1) && defined(OCIE1A)
        case 1:
            bitWrite(TIMSK1, OCIE1A, 0);
            break;
#endif

        case 2:
#if defined(TIMSK2) && defined(OCIE2A)
            bitWrite(TIMSK2, OCIE2A, 0); // disable interrupt
#endif
#if defined(TCCR2A) && defined(WGM20)
            TCCR2A = (1 << WGM20);
#endif
#if defined(TCCR2B) && defined(CS22)
            TCCR2B = (TCCR2B & 0b11111000) | (1 << CS22);
#endif
#if defined(OCR2A)
            OCR2A = 0;
#endif
            break;

#if defined(TIMSK3) && defined(OCIE3A)
        case 3:
            bitWrite(TIMSK3, OCIE3A, 0);
            break;
#endif

#if defined(TIMSK4) && defined(OCIE4A)
        case 4:
            bitWrite(TIMSK4, OCIE4A, 0);
            break;
#endif

#if defined(TIMSK5) && defined(OCIE5A)
        case 5:
            bitWrite(TIMSK5, OCIE5A, 0);
            break;
#endif
    }
}


void noTone(uint8_t _pin) {
    int8_t _timer = -1;

    for (int i = 0; i < AVAILABLE_TONE_PINS; i++) {
        if (tone_pins[i] == _pin) {
            _timer = pgm_read_byte(tone_pin_to_timer_PGM + i);
            tone_pins[i] = 255;
            break;
        }
    }

    disableTimer(_timer);

    digitalWrite(_pin, 0);
}

#ifdef USE_TIMER0
ISR(TIMER0_COMPA_vect) {
    if (timer0_toggle_count != 0) {
        // 拨动别针
        *timer0_pin_port ^= timer0_pin_mask;
        if (timer0_toggle_count > 0)
            timer0_toggle_count--;
    } else {
        disableTimer(0);
        *timer0_pin_port &= ~(timer0_pin_mask);  // 停止后保持低电平
    }
}
#endif


#ifdef USE_TIMER1
ISR(TIMER1_COMPA_vect) {
    if (timer1_toggle_count != 0) {
        // 拨动别针
        *timer1_pin_port ^= timer1_pin_mask;
        if (timer1_toggle_count > 0)
            timer1_toggle_count--;
    } else {
        disableTimer(1);
        *timer1_pin_port &= ~(timer1_pin_mask);  // 停止后保持低电平
    }
}
#endif


#ifdef USE_TIMER2
ISR(TIMER2_COMPA_vect) {
    if (timer2_toggle_count != 0) {
        // 拨动别针
        *timer2_pin_port ^= timer2_pin_mask;
        if (timer2_toggle_count > 0)
            timer2_toggle_count--;
    } else {
        // 需要调用noTone（）以便将tone_pins []条目重置，以便在下次我们调用tone（）时初始化计时器。
        // 假设计时器2始终是第一个使用的计时器。
        noTone(tone_pins[0]);
    }
}
#endif


#ifdef USE_TIMER3
ISR(TIMER3_COMPA_vect) {
    if (timer3_toggle_count != 0) {
        // 拨动别针
        *timer3_pin_port ^= timer3_pin_mask;
        if (timer3_toggle_count > 0)
            timer3_toggle_count--;
    } else {
        disableTimer(3);
        *timer3_pin_port &= ~(timer3_pin_mask);  // 停止后保持低电平
    }
}
#endif


#ifdef USE_TIMER4
ISR(TIMER4_COMPA_vect) {
    if (timer4_toggle_count != 0) {
        // 拨动别针
        *timer4_pin_port ^= timer4_pin_mask;
        if (timer4_toggle_count > 0)
            timer4_toggle_count--;
    } else {
        disableTimer(4);
        *timer4_pin_port &= ~(timer4_pin_mask);  // 停止后保持低电平
    }
}
#endif


#ifdef USE_TIMER5
ISR(TIMER5_COMPA_vect) {
    if (timer5_toggle_count != 0) {
        // 拨动别针
        *timer5_pin_port ^= timer5_pin_mask;
        if (timer5_toggle_count > 0)
            timer5_toggle_count--;
    } else {
        disableTimer(5);
        *timer5_pin_port &= ~(timer5_pin_mask);  // 停止后保持低电平
    }
}
#endif
