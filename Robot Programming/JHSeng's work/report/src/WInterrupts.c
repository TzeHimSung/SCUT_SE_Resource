#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include "wiring_private.h"
static void nothing(void) {}
static volatile voidFuncPtr intFunc[EXTERNAL_NUM_INTERRUPTS] = {
#if EXTERNAL_NUM_INTERRUPTS > 8
#warning There are more than 8 external interrupts. Some callbacks may not be initialized.
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 7
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 6
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 5
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 4
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 3
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 2
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 1
    nothing,
#endif
#if EXTERNAL_NUM_INTERRUPTS > 0
    nothing,
#endif
};
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode) {
    if (interruptNum < EXTERNAL_NUM_INTERRUPTS) {
        intFunc[interruptNum] = userFunc;
        // 配置中断模式（在低输入，任何变化，上升沿或下降沿触发）。
        // 选择模式常数以对应于硬件寄存器中的配置位，因此我们只需将模式移到适当位置以启用中断。
        switch (interruptNum) {
#if defined(__AVR_ATmega32U4__)
            // 寄存器分配在1280/2560和32U4之间有所不同。
            // 由于avrlib定义了32U4上甚至不存在的寄存器PCMSK1和PCMSK2，因此这是区分它们的唯一方法。
            case 0:
                EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
                EIMSK |= (1 << INT0);
                break;
            case 1:
                EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
                EIMSK |= (1 << INT1);
                break;
            case 2:
                EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
                EIMSK |= (1 << INT2);
                break;
            case 3:
                EICRA = (EICRA & ~((1 << ISC30) | (1 << ISC31))) | (mode << ISC30);
                EIMSK |= (1 << INT3);
                break;
            case 4:
                EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
                EIMSK |= (1 << INT6);
                break;
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
            case 2:
                EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
                EIMSK |= (1 << INT0);
                break;
            case 3:
                EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
                EIMSK |= (1 << INT1);
                break;
            case 4:
                EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
                EIMSK |= (1 << INT2);
                break;
            case 5:
                EICRA = (EICRA & ~((1 << ISC30) | (1 << ISC31))) | (mode << ISC30);
                EIMSK |= (1 << INT3);
                break;
            case 0:
                EICRB = (EICRB & ~((1 << ISC40) | (1 << ISC41))) | (mode << ISC40);
                EIMSK |= (1 << INT4);
                break;
            case 1:
                EICRB = (EICRB & ~((1 << ISC50) | (1 << ISC51))) | (mode << ISC50);
                EIMSK |= (1 << INT5);
                break;
            case 6:
                EICRB = (EICRB & ~((1 << ISC60) | (1 << ISC61))) | (mode << ISC60);
                EIMSK |= (1 << INT6);
                break;
            case 7:
                EICRB = (EICRB & ~((1 << ISC70) | (1 << ISC71))) | (mode << ISC70);
                EIMSK |= (1 << INT7);
                break;
#else
            case 0:
#if defined(EICRA) && defined(ISC00) && defined(EIMSK)
                EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
                EIMSK |= (1 << INT0);
#elif defined(MCUCR) && defined(ISC00) && defined(GICR)
                MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
                GICR |= (1 << INT0);
#elif defined(MCUCR) && defined(ISC00) && defined(GIMSK)
                MCUCR = (MCUCR & ~((1 << ISC00) | (1 << ISC01))) | (mode << ISC00);
                GIMSK |= (1 << INT0);
#else
#error attachInterrupt not finished for this CPU (case 0)
#endif
                break;

            case 1:
#if defined(EICRA) && defined(ISC10) && defined(ISC11) && defined(EIMSK)
                EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
                EIMSK |= (1 << INT1);
#elif defined(MCUCR) && defined(ISC10) && defined(ISC11) && defined(GICR)
                MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
                GICR |= (1 << INT1);
#elif defined(MCUCR) && defined(ISC10) && defined(GIMSK) && defined(GIMSK)
                MCUCR = (MCUCR & ~((1 << ISC10) | (1 << ISC11))) | (mode << ISC10);
                GIMSK |= (1 << INT1);
#else
#warning attachInterrupt may need some more work for this cpu (case 1)
#endif
                break;
            case 2:
#if defined(EICRA) && defined(ISC20) && defined(ISC21) && defined(EIMSK)
                EICRA = (EICRA & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
                EIMSK |= (1 << INT2);
#elif defined(MCUCR) && defined(ISC20) && defined(ISC21) && defined(GICR)
                MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
                GICR |= (1 << INT2);
#elif defined(MCUCR) && defined(ISC20) && defined(GIMSK) && defined(GIMSK)
                MCUCR = (MCUCR & ~((1 << ISC20) | (1 << ISC21))) | (mode << ISC20);
                GIMSK |= (1 << INT2);
#endif
                break;
#endif
        }
    }
}
void detachInterrupt(uint8_t interruptNum) {
    if (interruptNum < EXTERNAL_NUM_INTERRUPTS) {
        //禁用中断。我们不能假设interruptNum等于清除要清除的EIMSK位的编号，因为在ATmega8,INT0是6而INT1是7
        switch (interruptNum) {
#if defined(__AVR_ATmega32U4__)
            case 0:
                EIMSK &= ~(1 << INT0);
                break;
            case 1:
                EIMSK &= ~(1 << INT1);
                break;
            case 2:
                EIMSK &= ~(1 << INT2);
                break;
            case 3:
                EIMSK &= ~(1 << INT3);
                break;
            case 4:
                EIMSK &= ~(1 << INT6);
                break;
#elif defined(EICRA) && defined(EICRB) && defined(EIMSK)
            case 2:
                EIMSK &= ~(1 << INT0);
                break;
            case 3:
                EIMSK &= ~(1 << INT1);
                break;
            case 4:
                EIMSK &= ~(1 << INT2);
                break;
            case 5:
                EIMSK &= ~(1 << INT3);
                break;
            case 0:
                EIMSK &= ~(1 << INT4);
                break;
            case 1:
                EIMSK &= ~(1 << INT5);
                break;
            case 6:
                EIMSK &= ~(1 << INT6);
                break;
            case 7:
                EIMSK &= ~(1 << INT7);
                break;
#else
            case 0:
#if defined(EIMSK) && defined(INT0)
                EIMSK &= ~(1 << INT0);
#elif defined(GICR) && defined(ISC00)
                GICR &= ~(1 << INT0); // atmega32
#elif defined(GIMSK) && defined(INT0)
                GIMSK &= ~(1 << INT0);
#else
#error detachInterrupt not finished for this cpu
#endif
                break;

            case 1:
#if defined(EIMSK) && defined(INT1)
                EIMSK &= ~(1 << INT1);
#elif defined(GICR) && defined(INT1)
                GICR &= ~(1 << INT1); // atmega32
#elif defined(GIMSK) && defined(INT1)
                GIMSK &= ~(1 << INT1);
#else
#warning detachInterrupt may need some more work for this cpu (case 1)
#endif
                break;
            case 2:
#if defined(EIMSK) && defined(INT2)
                EIMSK &= ~(1 << INT2);
#elif defined(GICR) && defined(INT2)
                GICR &= ~(1 << INT2); // atmega32
#elif defined(GIMSK) && defined(INT2)
                GIMSK &= ~(1 << INT2);
#elif defined(INT2)
#warning detachInterrupt may need some more work for this cpu (case 2)
#endif
                break;
#endif
        }
        intFunc[interruptNum] = nothing;
    }
}
#define IMPLEMENT_ISR(vect, interrupt) \
  ISR(vect) { \
    intFunc[interrupt](); \
  }
#if defined(__AVR_ATmega32U4__)
IMPLEMENT_ISR(INT0_vect, EXTERNAL_INT_0)
IMPLEMENT_ISR(INT1_vect, EXTERNAL_INT_1)
IMPLEMENT_ISR(INT2_vect, EXTERNAL_INT_2)
IMPLEMENT_ISR(INT3_vect, EXTERNAL_INT_3)
IMPLEMENT_ISR(INT6_vect, EXTERNAL_INT_4)
#elif defined(EICRA) && defined(EICRB)
IMPLEMENT_ISR(INT0_vect, EXTERNAL_INT_2)
IMPLEMENT_ISR(INT1_vect, EXTERNAL_INT_3)
IMPLEMENT_ISR(INT2_vect, EXTERNAL_INT_4)
IMPLEMENT_ISR(INT3_vect, EXTERNAL_INT_5)
IMPLEMENT_ISR(INT4_vect, EXTERNAL_INT_0)
IMPLEMENT_ISR(INT5_vect, EXTERNAL_INT_1)
IMPLEMENT_ISR(INT6_vect, EXTERNAL_INT_6)
IMPLEMENT_ISR(INT7_vect, EXTERNAL_INT_7)
#else
IMPLEMENT_ISR(INT0_vect, EXTERNAL_INT_0)
IMPLEMENT_ISR(INT1_vect, EXTERNAL_INT_1)
#if defined(EICRA) && defined(ISC20)
IMPLEMENT_ISR(INT2_vect, EXTERNAL_INT_2)
#endif

#endif
