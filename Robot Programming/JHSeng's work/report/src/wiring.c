#include "wiring_private.h"
// 设置预分频器，以便timer0每64个时钟周期滴答一次，而溢出处理程序每256个滴答调用一次。
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
// 每个计时器的总毫秒数0溢出
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)
// 每个定时器0溢出的小数毫秒数。 我们右移三位以使这些数字适合一个字节。 (对于我们关心的时钟速度-8和16 MHz-这不会丢失精度。)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;
#if defined(TIM0_OVF_vect)
ISR(TIM0_OVF_vect)
#else
ISR(TIMER0_OVF_vect)
#endif
{
    // 将它们复制到局部变量，以便可以将它们存储在寄存器中(每次访问都必须从内存中读取易失性变量)
    unsigned long m = timer0_millis;
    unsigned char f = timer0_fract;
    m += MILLIS_INC;
    f += FRACT_INC;
    if (f >= FRACT_MAX) {
        f -= FRACT_MAX;
        m += 1;
    }
    timer0_fract = f;
    timer0_millis = m;
    timer0_overflow_count++;
}
unsigned long millis() {
    unsigned long m;
    uint8_t oldSREG = SREG;
    // 在读取timer0_millis时禁用中断，否则我们可能会获得不一致的值(例如，在写入timer0_millis的过程中)
    cli();
    m = timer0_millis;
    SREG = oldSREG;
    return m;
}
unsigned long micros() {
    unsigned long m;
    uint8_t oldSREG = SREG, t;
    cli();
    m = timer0_overflow_count;
#if defined(TCNT0)
    t = TCNT0;
#elif defined(TCNT0L)
    t = TCNT0L;
#else
#error TIMER 0 not defined
#endif

#ifdef TIFR0
    if ((TIFR0 & _BV(TOV0)) && (t < 255)) m++;
#else
    if ((TIFR & _BV(TOV0)) && (t < 255)) m++;
#endif
    SREG = oldSREG;
    return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}
void delay(unsigned long ms) {
    uint32_t start = micros();
    while (ms > 0) {
        yield();
        while ( ms > 0 && (micros() - start) >= 1000) {
            ms--;
            start += 1000;
        }
    }
}
// 延迟给定的微秒数。 假设1、8、12、16、20或24 MHz时钟。
void delayMicroseconds(unsigned int us) {
    // 调用= 4个周期+ 2到4个周期来初始化us(2表示恒定延迟，4表示变量)
    // 以较低的值(例如1或2微秒)呼叫avrlib的delay_us()函数会产生比预期更长的延迟。
#if F_CPU >= 24000000L
    // 对于有冒险精神的24 MHz时钟，尝试超频零延迟修复
    if (!us) return;
    // 接下来的循环每次迭代花费1/6微秒(4个周期)，因此对于所请求的每微秒延迟执行6次。
    us *= 6;
    // 占前面命令中花费的时间。 我们只是烧掉了22(24)个以上的周期，删除了5个(5 * 4 = 20)我们至少是6个，所以我们可以减去5个
    us -= 5;
#elif F_CPU >= 20000000L
    // 用于罕见的Arduino板上的20 MHz时钟
    // 延迟一微秒，只需返回即可。 函数调用的开销需要18(20)个周期，即1us
    __asm__ __volatile__ (
        "nop" "\n\t"
        "nop" "\n\t"
        "nop" "\n\t"
        "nop"); // 只等了4个周期
    if (us <= 1) return;
    // 接下来的循环每次迭代花费1/5微秒(4个周期)，因此对于所请求的每微秒延迟执行5次。
    us = (us << 2) + us;
    // 占前面命令中花费的时间。
    // 我们只燃烧了26(28)个以上的周期，删除了7(7 * 4 = 28)个至少10个，所以我们可以减去7
    us -= 7;
#elif F_CPU >= 16000000L
    // 适用于大多数Arduino开发板上的16 MHz时钟
    // 延迟一微秒，只需返回即可。 函数调用的开销需要14(16)个周期，即1us
    if (us <= 1) return;
    // 接下来的循环每次迭代需要1/4微秒(4个周期)，因此对于请求的每微秒延迟，请执行四次。
    us <<= 2;
    // 占前面命令中花费的时间。
    // 我们只是烧掉了上面的19(21)个周期，删除了5(5 * 4 = 20)个，所以我们至少是8，所以我们可以减去5
    us -= 5;
#elif F_CPU >= 12000000L
    // 如果有人使用USB，则为12 MHz时钟
    // 对于1微秒的延迟，只需返回即可。 函数调用的开销需要14(16)个周期，即1.5us
    if (us <= 1) return;
    // 接下来的循环每次迭代耗时1/3微秒(4个周期)，因此对于所请求的每微秒延迟执行3次。
    us = (us << 1) + us;
    // 占前面命令中花费的时间。
    // 我们只烧了20(22)个以上的周期，删除5，(5 * 4 = 20)我们至少是6，所以我们可以减去5
    us -= 5;
#elif F_CPU >= 8000000L
    // 用于8 MHz内部时钟
    // 对于1和2微秒的延迟，只需返回即可。 函数调用的开销需要14(16)个周期，即2us
    if (us <= 2) return;
    // 接下来的循环每次迭代需要1/2微秒(4个周期)，因此对于所请求的每微秒延迟执行两次。
    us <<= 1; //x2 us, = 2 cycles
    // 占前面命令中花费的时间。
    // 我们只是在上面烧了17(19)个周期，删除4，(4 * 4 = 16)，我们至少是6，所以我们可以减去4
    us -= 4;
#else
    // 用于1 MHz内部时钟(常见的Atmega微控制器的默认设置)
    // 函数调用的开销为14(16)周期
    if (us <= 16) return
            if (us <= 25) return, (must be at least 25 if we want to substract 22)

                    // 补偿上一条和下一条命令所花费的时间(大约22个周期)
                    us -= 22;
    // 接下来的循环每次迭代需要4微秒(4个周期)，因此执行us / 4至少为4，除以4得到1(没有零延迟错误)
    us >>= 2;
#endif
    // 忙等待
    __asm__ __volatile__ (
        "1: sbiw %0,1" "\n\t"
        "brne 1b" : "=w" (us) : "0" (us)
    );
}
void init() {
    // 这需要在setup()之前调用，否则某些功能将无法正常运行
    sei();
    // 在ATmega168上，定时器0也用于快速硬件pwm(使用相位校正PWM意味着定时器0溢出的频率是原来的一半，导致ATmega8和ATmega168上的millis()行为不同)
#if defined(TCCR0A) && defined(WGM01)
    sbi(TCCR0A, WGM01);
    sbi(TCCR0A, WGM00);
#endif
    // 将定时器0预分频因子设置为64
#if defined(__AVR_ATmega128__)
    // CPU特定：ATmega128的不同值
    sbi(TCCR0, CS02);
#elif defined(TCCR0) && defined(CS01) && defined(CS00)
    // 适用于 atmega8 的标准
    sbi(TCCR0, CS01);
    sbi(TCCR0, CS00);
#elif defined(TCCR0B) && defined(CS01) && defined(CS00)
    // 适用于 168/328/1280/2560 的标准
    sbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);
#elif defined(TCCR0A) && defined(CS01) && defined(CS00)
    // 适用于 __AVR_ATmega645__ 系列
    sbi(TCCR0A, CS01);
    sbi(TCCR0A, CS00);
#else
#error Timer 0 prescale factor 64 not set correctly
#endif

    // 启用定时器0溢出中断
#if defined(TIMSK) && defined(TOIE0)
    sbi(TIMSK, TOIE0);
#elif defined(TIMSK0) && defined(TOIE0)
    sbi(TIMSK0, TOIE0);
#else
#error  Timer 0 overflow interrupt not set correctly
#endif
    // 定时器1和2用于相位校正硬件pwm
    // 这对于电动机来说更好，因为它可以确保均匀的波形记录，但是，快速的pwm模式可以在占空比为50％的情况下达到高达8 MHz的频率(时钟为16 MHz)
#if defined(TCCR1B) && defined(CS11) && defined(CS10)
    TCCR1B = 0;
    // 将计时器1的预分频因子设置为64
    sbi(TCCR1B, CS11);
#if F_CPU >= 8000000L
    sbi(TCCR1B, CS10);
#endif
#elif defined(TCCR1) && defined(CS11) && defined(CS10)
    sbi(TCCR1, CS11);
#if F_CPU >= 8000000L
    sbi(TCCR1, CS10);
#endif
#endif
    // 将定时器1置于8位相位校正pwm模式
#if defined(TCCR1A) && defined(WGM10)
    sbi(TCCR1A, WGM10);
#endif

    // 将计时器2的预分频因子设置为64
#if defined(TCCR2) && defined(CS22)
    sbi(TCCR2, CS22);
#elif defined(TCCR2B) && defined(CS22)
    sbi(TCCR2B, CS22);
//#else
    // 计时器2未完成(此CPU上可能不存在)
#endif

    // 将定时器2置于8位相位校正pwm模式
#if defined(TCCR2) && defined(WGM20)
    sbi(TCCR2, WGM20);
#elif defined(TCCR2A) && defined(WGM20)
    sbi(TCCR2A, WGM20);
//#else
    // 计时器2未完成(此CPU上可能不存在)
#endif

#if defined(TCCR3B) && defined(CS31) && defined(WGM30)
    sbi(TCCR3B, CS31);      // 将计时器3的预分频因子设置为64
    sbi(TCCR3B, CS30);
    sbi(TCCR3A, WGM30);     // 将定时器3置于8位相位校正pwm模式
#endif

#if defined(TCCR4A) && defined(TCCR4B) && defined(TCCR4D) /* beginning of timer4 block for 32U4 and similar */
    sbi(TCCR4B, CS42);      // 将计时器4的预分频因子设置为64
    sbi(TCCR4B, CS41);
    sbi(TCCR4B, CS40);
    sbi(TCCR4D, WGM40);     // 将计时器4置于相位和频率校正PWM模式
    sbi(TCCR4A, PWM4A);     // 使能比较器OCR4A的PWM模式
    sbi(TCCR4C, PWM4D);     // 为比较器OCR4D启用PWM模式
#else /* ATMEGA1280和ATMEGA2560的timer4块的开始 */
#if defined(TCCR4B) && defined(CS41) && defined(WGM40)
    sbi(TCCR4B, CS41);      // 将计时器4的预分频因子设置为64
    sbi(TCCR4B, CS40);
    sbi(TCCR4A, WGM40);     // 将定时器4置于8位相位校正pwm模式
#endif
#endif /* 用于ATMEGA1280 / 2560和类似设备的end timer4块 */

#if defined(TCCR5B) && defined(CS51) && defined(WGM50)
    sbi(TCCR5B, CS51);      // 将计时器5的预分频因子设置为64
    sbi(TCCR5B, CS50);
    sbi(TCCR5A, WGM50);     // 将定时器5置于8位相位校正pwm模式
#endif

#if defined(ADCSRA)
    // 设置a2d预分频器，使我们处于所需的50-200 KHz范围内。
#if F_CPU >= 16000000
    sbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
#elif F_CPU >= 8000000
    sbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
#elif F_CPU >= 4000000
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
#elif F_CPU >= 2000000
    sbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    cbi(ADCSRA, ADPS0);
#elif F_CPU >= 1000000
    cbi(ADCSRA, ADPS2);
    sbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
#else
    cbi(ADCSRA, ADPS2);
    cbi(ADCSRA, ADPS1);
    sbi(ADCSRA, ADPS0);
#endif
    // 启用a2d转换
    sbi(ADCSRA, ADEN);
#endif
    // 引导程序将引脚0和1连接到USART;在这里断开它们的连接，以便可以将它们用作普通的数字输入/输出; 它们将在Serial.begin()中重新连接
#if defined(UCSRB)
    UCSRB = 0;
#elif defined(UCSR0B)
    UCSR0B = 0;
#endif
}
