#include "USBAPI.h"
#include "PluggableUSB.h"
#include <stdlib.h>
#if defined(USBCON)
/**脉冲生成计数器，用于跟踪每种脉冲类型剩余的毫秒数*/
#define TX_RX_LED_PULSE_MS 100
volatile u8 TxLEDPulse; /** <数据Tx LED脉冲剩余的毫秒数*/
volatile u8 RxLEDPulse; /** <剩余的毫秒数用于数据接收LED脉冲*/
extern const u16 STRING_LANGUAGE[] PROGMEM;
extern const u8 STRING_PRODUCT[] PROGMEM;
extern const u8 STRING_MANUFACTURER[] PROGMEM;
extern const DeviceDescriptor USB_DeviceDescriptorIAD PROGMEM;
const u16 STRING_LANGUAGE[2] = {
    (3 << 8) | (2 + 2),
    0x0409  // English
};
#ifndef USB_PRODUCT
// 如果未提供产品，使用USB IO板
#define USB_PRODUCT     "USB IO Board"
#endif
const u8 STRING_PRODUCT[] PROGMEM = USB_PRODUCT;
#if USB_VID == 0x2341
#  if defined(USB_MANUFACTURER)
#    undef USB_MANUFACTURER
#  endif
#  define USB_MANUFACTURER "Arduino LLC"
#elif USB_VID == 0x1b4f
#  if defined(USB_MANUFACTURER)
#    undef USB_MANUFACTURER
#  endif
#  define USB_MANUFACTURER "SparkFun"
#elif !defined(USB_MANUFACTURER)
// 如果宏中未提供制造商名称，则失败
#  define USB_MANUFACTURER "Unknown"
#endif
const u8 STRING_MANUFACTURER[] PROGMEM = USB_MANUFACTURER;
#define DEVICE_CLASS 0x02
// 设备描述器
const DeviceDescriptor USB_DeviceDescriptorIAD =
    D_DEVICE(0xEF, 0x02, 0x01, 64, USB_VID, USB_PID, 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);
volatile u8 _usbConfiguration = 0;
volatile u8 _usbCurrentStatus = 0; // 由GetStatus()请求返回到设备的信息
volatile u8 _usbSuspendState = 0; // UDINT的副本以检查SUSPI和WAKEUPI位
static inline void WaitIN(void) {
    while (!(UEINTX & (1 << TXINI)));
}
static inline void ClearIN(void) {
    UEINTX = ~(1 << TXINI);
}
static inline void WaitOUT(void) {
    while (!(UEINTX & (1 << RXOUTI)));
}
static inline u8 WaitForINOrOUT() {
    while (!(UEINTX & ((1 << TXINI) | (1 << RXOUTI))));
    return (UEINTX & (1 << RXOUTI)) == 0;
}
static inline void ClearOUT(void) {
    UEINTX = ~(1 << RXOUTI);
}
static inline void Recv(volatile u8 *data, u8 count) {
    while (count--) *data++ = UEDATX;
    RXLED1;                 // 点亮RX LED
    RxLEDPulse = TX_RX_LED_PULSE_MS;
}
static inline u8 Recv8() {
    RXLED1;                 // 点亮RX LED
    RxLEDPulse = TX_RX_LED_PULSE_MS;
    return UEDATX;
}
static inline void Send8(u8 d) {
    UEDATX = d;
}
static inline void SetEP(u8 ep) {
    UENUM = ep;
}
static inline u8 FifoByteCount() {
    return UEBCLX;
}
static inline u8 ReceivedSetupInt() {
    return UEINTX & (1 << RXSTPI);
}
static inline void ClearSetupInt() {
    UEINTX = ~((1 << RXSTPI) | (1 << RXOUTI) | (1 << TXINI));
}
static inline void Stall() {
    UECONX = (1 << STALLRQ) | (1 << EPEN);
}
static inline u8 ReadWriteAllowed() {
    return UEINTX & (1 << RWAL);
}
static inline u8 Stalled() {
    return UEINTX & (1 << STALLEDI);
}
static inline u8 FifoFree() {
    return UEINTX & (1 << FIFOCON);
}
static inline void ReleaseRX() {
    UEINTX = 0x6B;  // FIFOCON=0 NAKINI=1 RWAL=1 NAKOUTI=0 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=1
}
static inline void ReleaseTX() {
    UEINTX = 0x3A;  // FIFOCON=0 NAKINI=0 RWAL=1 NAKOUTI=1 RXSTPI=1 RXOUTI=0 STALLEDI=1 TXINI=0
}
static inline u8 FrameNumber() {
    return UDFNUML;
}
u8 USBGetConfiguration(void) {
    return _usbConfiguration;
}
#define USB_RECV_TIMEOUT
class LockEP {
    u8 _sreg;
public:
    LockEP(u8 ep) : _sreg(SREG) {
        cli();
        SetEP(ep & 7);
    }
    ~LockEP() {
        SREG = _sreg;
    }
};
// 字节数（假设为RX端点）
u8 USB_Available(u8 ep) {
    LockEP lock(ep);
    return FifoByteCount();
}
//  非阻塞接收
//  返回读取的字节数
int USB_Recv(u8 ep, void *d, int len) {
    if (!_usbConfiguration || len < 0) return -1;
    LockEP lock(ep);
    u8 n = FifoByteCount();
    len = min(n, len);
    n = len;
    u8 *dst = (u8 *)d;
    while (n--) *dst++ = Recv8();
    // 释放空缓冲区
    if (len && !FifoByteCount()) ReleaseRX();
    return len;
}
// 如果准备好，接收1byte
int USB_Recv(u8 ep) {
    u8 c;
    if (USB_Recv(ep, &c, 1) != 1) return -1;
    return c;
}
// 发送EP的空间
u8 USB_SendSpace(u8 ep) {
    LockEP lock(ep);
    if (!ReadWriteAllowed()) return 0;
    return USB_EP_SIZE - FifoByteCount();
}
//  禁止向端点发送数据
int USB_Send(u8 ep, const void *d, int len) {
    if (!_usbConfiguration) return -1;
    if (_usbSuspendState & (1 << SUSPI)) {
        // 发送远程唤醒
        UDCON |= (1 << RMWKUP);
    }
    int r = len;
    const u8 *data = (const u8 *)d;
    u8 timeout = 250;       // 250ms超时则再次发送
    bool sendZlp = false;
    while (len || sendZlp) {
        u8 n = USB_SendSpace(ep);
        if (n == 0) {
            if (!(--timeout)) return -1;
            delay(1);
            continue;
        }
        if (n > len) n = len;
        {
            LockEP lock(ep);
            // SOF中断处理程序可能已释放帧
            if (!ReadWriteAllowed()) continue;
            len -= n;
            if (ep & TRANSFER_ZERO) {
                while (n--) Send8(0);
            } else if (ep & TRANSFER_PGM) {
                while (n--) Send8(pgm_read_byte(data++));
            } else {
                while (n--) Send8(*data++);
            }
            if (sendZlp) {
                ReleaseTX();
                sendZlp = false;
            } else if (!ReadWriteAllowed()) { // 如果缓冲区满，释放之
                ReleaseTX();
                if (len == 0) sendZlp = true;
            } else if ((len == 0) && (ep & TRANSFER_RELEASE)) { // 用TRANSFER_RELEASE强行释放
                // 从未使用过TRANSFER_RELEASE，是否可以删除？
                ReleaseTX();
            }
        }
    }
    TXLED1;                 // 点亮TX LED
    TxLEDPulse = TX_RX_LED_PULSE_MS;
    return r;
}
u8 _initEndpoints[USB_ENDPOINTS] = {
    0,                      // 控制端点
    EP_TYPE_INTERRUPT_IN,   // CDC_ENDPOINT_ACM
    EP_TYPE_BULK_OUT,       // CDC_ENDPOINT_OUT
    EP_TYPE_BULK_IN,        // CDC_ENDPOINT_IN
    // 接下来的端点都被初始化为0
};
#define EP_SINGLE_64 0x32   // EP0
#define EP_DOUBLE_64 0x36   // 其他端点
#define EP_SINGLE_16 0x12
static
void InitEP(u8 index, u8 type, u8 size) {
    UENUM = index;
    UECONX = (1 << EPEN);
    UECFG0X = type;
    UECFG1X = size;
}
static
void InitEndpoints() {
    for (u8 i = 1; i < sizeof(_initEndpoints) && _initEndpoints[i] != 0; i++) {
        UENUM = i;
        UECONX = (1 << EPEN);
        UECFG0X = _initEndpoints[i];
#if USB_EP_SIZE == 16
        UECFG1X = EP_SINGLE_16;
#elif USB_EP_SIZE == 64
        UECFG1X = EP_DOUBLE_64;
#else
#error Unsupported value for USB_EP_SIZE
#endif
    }
    UERST = 0x7E;   // 重置它们
    UERST = 0;
}
// 处理CLASS_INTERFACE请求
static
bool ClassInterfaceRequest(USBSetup &setup) {
    u8 i = setup.wIndex;
    if (CDC_ACM_INTERFACE == i) return CDC_Setup(setup);
#ifdef PLUGGABLE_USB_ENABLED
    return PluggableUSB().setup(setup);
#endif
    return false;
}
static int _cmark;
static int _cend;
void InitControl(int end) {
    SetEP(0);
    _cmark = 0;
    _cend = end;
}
static
bool SendControl(u8 d) {
    if (_cmark < _cend) {
        if (!WaitForINOrOUT()) return false;
        Send8(d);
        if (!((_cmark + 1) & 0x3F)) ClearIN();  // fifo满，释放之
    }
    _cmark++;
    return true;
}
//  被_cmark / _cend裁剪
int USB_SendControl(u8 flags, const void *d, int len) {
    int sent = len;
    const u8 *data = (const u8 *)d;
    bool pgm = flags & TRANSFER_PGM;
    while (len--) {
        u8 c = pgm ? pgm_read_byte(data++) : *data++;
        if (!SendControl(c))
            return -1;
    }
    return sent;
}
// 发送USB描述符字符串。该字符串作为一个纯ASCII字符串存储在PROGMEM中，以正确的2字节作为UTF-16发送字首
static bool USB_SendStringDescriptor(const u8 *string_P, u8 string_len, uint8_t flags) {
    SendControl(2 + string_len * 2);
    SendControl(3);
    bool pgm = flags & TRANSFER_PGM;
    for (u8 i = 0; i < string_len; i++) {
        bool r = SendControl(pgm ? pgm_read_byte(&string_P[i]) : string_P[i]);
        r &= SendControl(0); // 高位
        if (!r) return false;
    }
    return true;
}
// 不会超时或跨越FIFO边界
int USB_RecvControl(void *d, int len) {
    auto length = len;
    while (length) {
        // 收到的收益不超过USB Control EP所能提供的。使用固定的64，因为控制EP即使在16u2上也始终具有64个字节。
        auto recvLength = length;
        if (recvLength > 64) recvLength = 64;
        // 写入数据以适合数组的结尾（而不是开头）
        WaitOUT();
        Recv((u8 *)d + len - length, recvLength);
        ClearOUT();
        length -= recvLength;
    }
    return len;
}
static u8 SendInterfaces() {
    u8 interfaces = 0;
    CDC_GetInterface(&interfaces);
#ifdef PLUGGABLE_USB_ENABLED
    PluggableUSB().getInterface(&interfaces);
#endif
    return interfaces;
}
// 构造动态配置描述符，需要动态的端点分配等
static
bool SendConfiguration(int maxlen) {
    // 计数和度量接口
    InitControl(0);
    u8 interfaces = SendInterfaces();
    ConfigDescriptor config = D_CONFIG(_cmark + sizeof(ConfigDescriptor), interfaces);
    // 发送数据
    InitControl(maxlen);
    USB_SendControl(0, &config, sizeof(ConfigDescriptor));
    SendInterfaces();
    return true;
}
static
bool SendDescriptor(USBSetup &setup) {
    int ret;
    u8 t = setup.wValueH;
    if (USB_CONFIGURATION_DESCRIPTOR_TYPE == t) return SendConfiguration(setup.wLength);
    InitControl(setup.wLength);
#ifdef PLUGGABLE_USB_ENABLED
    ret = PluggableUSB().getDescriptor(setup);
    if (ret != 0) return (ret > 0 ? true : false);
#endif
    const u8 *desc_addr = 0;
    if (USB_DEVICE_DESCRIPTOR_TYPE == t) {
        desc_addr = (const u8 *)&USB_DeviceDescriptorIAD;
    } else if (USB_STRING_DESCRIPTOR_TYPE == t) {
        if (setup.wValueL == 0) {
            desc_addr = (const u8 *)&STRING_LANGUAGE;
        } else if (setup.wValueL == IPRODUCT) {
            return USB_SendStringDescriptor(STRING_PRODUCT, strlen(USB_PRODUCT), TRANSFER_PGM);
        } else if (setup.wValueL == IMANUFACTURER) {
            return USB_SendStringDescriptor(STRING_MANUFACTURER, strlen(USB_MANUFACTURER), TRANSFER_PGM);
        } else if (setup.wValueL == ISERIAL) {
#ifdef PLUGGABLE_USB_ENABLED
            char name[ISERIAL_MAX_LEN];
            PluggableUSB().getShortName(name);
            return USB_SendStringDescriptor((uint8_t *)name, strlen(name), 0);
#endif
        } else return false;
    }
    if (desc_addr == 0) return false;
    u8 desc_length = pgm_read_byte(desc_addr);
    USB_SendControl(TRANSFER_PGM, desc_addr, desc_length);
    return true;
}
// 端点0中断
ISR(USB_COM_vect) {
    SetEP(0);
    if (!ReceivedSetupInt()) return;
    USBSetup setup;
    Recv((u8 *)&setup, 8);
    ClearSetupInt();
    u8 requestType = setup.bmRequestType;
    if (requestType & REQUEST_DEVICETOHOST) WaitIN();
    else ClearIN();
    bool ok = true;
    if (REQUEST_STANDARD == (requestType & REQUEST_TYPE)) {
        // 标准请求
        u8 r = setup.bRequest;
        u16 wValue = setup.wValueL | (setup.wValueH << 8);
        if (GET_STATUS == r) {
            if (requestType == (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_DEVICE)) {
                Send8(_usbCurrentStatus);
                Send8(0);
            } else {
                // TODO：在此处处理端点的HALT状态
                Send8(0); Send8(0);
            }
        } else if (CLEAR_FEATURE == r) {
            if ((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
                    && (wValue == DEVICE_REMOTE_WAKEUP)) {
                _usbCurrentStatus &= ~FEATURE_REMOTE_WAKEUP_ENABLED;
            }
        } else if (SET_FEATURE == r) {
            if ((requestType == (REQUEST_HOSTTODEVICE | REQUEST_STANDARD | REQUEST_DEVICE))
                    && (wValue == DEVICE_REMOTE_WAKEUP)) {
                _usbCurrentStatus |= FEATURE_REMOTE_WAKEUP_ENABLED;
            }
        } else if (SET_ADDRESS == r) {
            WaitIN();
            UDADDR = setup.wValueL | (1 << ADDEN);
        } else if (GET_DESCRIPTOR == r) {
            ok = SendDescriptor(setup);
        } else if (SET_DESCRIPTOR == r) {
            ok = false;
        } else if (GET_CONFIGURATION == r) {
            Send8(1);
        } else if (SET_CONFIGURATION == r) {
            if (REQUEST_DEVICE == (requestType & REQUEST_RECIPIENT)) {
                InitEndpoints();
                _usbConfiguration = setup.wValueL;
            } else
                ok = false;
        } else if (GET_INTERFACE == r) {
        } else if (SET_INTERFACE == r) {
        }
    } else {
        InitControl(setup.wLength);     //  传输的最大容量
        ok = ClassInterfaceRequest(setup);
    }
    if (ok) ClearIN();
    else Stall();
}
void USB_Flush(u8 ep) {
    SetEP(ep);
    if (FifoByteCount()) ReleaseTX();
}
static inline void USB_ClockDisable() {
#if defined(OTGPADE)
    USBCON = (USBCON & ~(1 << OTGPADE)) | (1 << FRZCLK); // 冻结时钟并禁用VBUS Pad
#else // u2系列
    USBCON = (1 << FRZCLK); // 冻结时钟
#endif
    PLLCSR &= ~(1 << PLLE); // 停止PLL
}
static inline void USB_ClockEnable() {
#if defined(UHWCON)
    UHWCON |= (1 << UVREGE);        // 电源内部寄存器
#endif
    USBCON = (1 << USBE) | (1 << FRZCLK); // 时钟冻结，USB启用
// ATmega32U4
#if defined(PINDIV)
#if F_CPU == 16000000UL
    PLLCSR |= (1 << PINDIV);                 // 需要 16 MHz xtal
#elif F_CPU == 8000000UL
    PLLCSR &= ~(1 << PINDIV);                // 需要  8 MHz xtal
#else
#error "Clock rate of F_CPU not supported"
#endif
#elif defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega8U2__)
    // 对于u2系列，数据表令人困惑。 在第40页上称为PINDIV，在第290页上称为PLLP0
#if F_CPU == 16000000UL
    // 需要 16 MHz xtal
    PLLCSR |= (1 << PLLP0);
#elif F_CPU == 8000000UL
    // 需要 8 MHz xtal
    PLLCSR &= ~(1 << PLLP0);
#endif
// AT90USB646, AT90USB647, AT90USB1286, AT90USB1287
#elif defined(PLLP2)
#if F_CPU == 16000000UL
#if defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__)
    // 用于Atmel AT90USB128x. 不要用于 Atmel AT90USB64x.
    PLLCSR = (PLLCSR & ~(1 << PLLP1)) | ((1 << PLLP2) | (1 << PLLP0)); // 需要 16 MHz xtal
#elif defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__)
    // 用于AT90USB64x. 不要用于 AT90USB128x.
    PLLCSR = (PLLCSR & ~(1 << PLLP0)) | ((1 << PLLP2) | (1 << PLLP1)); // 需要 16 MHz xtal
#else
#error "USB Chip not supported, please defined method of USB PLL initialization"
#endif
#elif F_CPU == 8000000UL
    // 用于Atmel AT90USB128x and AT90USB64x
    PLLCSR = (PLLCSR & ~(1 << PLLP2)) | ((1 << PLLP1) | (1 << PLLP0)); // 需要 8 MHz xtal
#else
#error "Clock rate of F_CPU not supported"
#endif
#else
#error "USB Chip not supported, please defined method of USB PLL initialization"
#endif
    PLLCSR |= (1 << PLLE);
    while (!(PLLCSR & (1 << PLOCK))) {}  // 等待锁定
    // 在特定版本的macosx（10.7.3）上进行了一些测试，报告了一些使用串行重置板时的奇怪行为
    // 通过端口触摸速度为1200 bps。 此延迟可以解决此问题。
    delay(1);
#if defined(OTGPADE)
    USBCON = (USBCON & ~(1 << FRZCLK)) | (1 << OTGPADE); // 启动USB clock, enable VBUS Pad
#else
    USBCON &= ~(1 << FRZCLK);   // 启动USB clock
#endif
#if defined(RSTCPU)
#if defined(LSM)
    UDCON &= ~((1 << RSTCPU) | (1 << LSM) | (1 << RMWKUP) | (1 << DETACH)); // 启用连接电阻，设置全速模式
#else // u2 Series
    UDCON &= ~((1 << RSTCPU) | (1 << RMWKUP) | (1 << DETACH));  // 启用连接电阻，设置全速模式
#endif
#else
    // AT90USB64x和AT90USB128x没有RSTCPU
    UDCON &= ~((1 << LSM) | (1 << RMWKUP) | (1 << DETACH)); // 启用连接电阻，设置全速模式
#endif
}
// 普通中断
ISR(USB_GEN_vect) {
    u8 udint = UDINT;
    UDINT &= ~((1 << EORSTI) | (1 << SOFI)); // 清除此处要处理的IRQ的IRQ标志，WAKEUPI和SUSPI除外
    // 重置
    if (udint & (1 << EORSTI)) {
        InitEP(0, EP_TYPE_CONTROL, EP_SINGLE_64); // 初始化ep0
        _usbConfiguration = 0;          // 没有设置
        UEIENX = 1 << RXSTPE;           // 为ep0启用中断
    }
    //  启动Frame。每毫秒发生一次，因此我们也将其用于TX和RX LED一次触发时序
    if (udint & (1 << SOFI)) {
        USB_Flush(CDC_TX);              // 如果找到，发送一个tx帧
        // 检查一次捕获是否已经过去。 如果是这样，请关闭LED
        if (TxLEDPulse && !(--TxLEDPulse)) TXLED0;
        if (RxLEDPulse && !(--RxLEDPulse)) RXLED0;
    }
    // 一旦数据上出现非空闲模式，就会触发WAKEUPI中断行。 因此，即使控制器未处于“挂起”模式，也可能发生WAKEUPI中断。
    // 因此，我们仅在USB挂起时才启用它。
    if (udint & (1 << WAKEUPI)) {
        UDIEN = (UDIEN & ~(1 << WAKEUPE)) | (1 << SUSPE); // 禁用WAKEUP中断，并允许SUSPEND中断
        // WAKEUPI应通过软件清除（必须先启用USB时钟输入）。
        // USB_ClockEnable();
        UDINT &= ~(1 << WAKEUPI);
        _usbSuspendState = (_usbSuspendState & ~(1 << SUSPI)) | (1 << WAKEUPI);
    } else if (udint & (1 << SUSPI)) { // WAKEUPI / SUSPI位中只有一个可以同时激活
        UDIEN = (UDIEN & ~(1 << SUSPE)) | (1 << WAKEUPE); // 禁用SUSPEND中断并启用WAKEUP中断
        //USB_ClockDisable();
        UDINT &= ~((1 << WAKEUPI) | (1 << SUSPI)); // 清除所有已经挂起的WAKEUP IRQ和SUSPI请求
        _usbSuspendState = (_usbSuspendState & ~(1 << WAKEUPI)) | (1 << SUSPI);
    }
}
//  VBUS或计数帧
u8 USBConnected() {
    u8 f = UDFNUML;
    delay(3);
    return f != UDFNUML;
}
USBDevice_ USBDevice;
USBDevice_::USBDevice_() {}
void USBDevice_::attach() {
    _usbConfiguration = 0;
    _usbCurrentStatus = 0;
    _usbSuspendState = 0;
    USB_ClockEnable();
    UDINT &= ~((1 << WAKEUPI) | (1 << SUSPI)); // 清除已经挂起的WAKEUP / SUSPEND请求
    UDIEN = (1 << EORSTE) | (1 << SOFE) | (1 << SUSPE); // 启用EOR（复位结束），SOF（帧启动）和SUSPEND中断
    TX_RX_LED_INIT;
}
void USBDevice_::detach() {}
//  检查中断。需要完成VBUS检测
bool USBDevice_::configured() {
    return _usbConfiguration;
}
void USBDevice_::poll() {}
bool USBDevice_::wakeupHost() {
    // 清除所有先前已设置但可以在该时间处理的唤醒请求。例如因为当时主机尚未暂停。
    UDCON &= ~(1 << RMWKUP);
    if (!(UDCON & (1 << RMWKUP))
            && (_usbSuspendState & (1 << SUSPI))
            && (_usbCurrentStatus & FEATURE_REMOTE_WAKEUP_ENABLED)) {
        // 此简短版本仅在尚未暂停设备时起作用。 目前Arduino核心根本不处理SUSPEND，所以没关系。
        USB_ClockEnable();
        UDCON |= (1 << RMWKUP); // 发送唤醒请求
        return true;
    }

    return false;
}
bool USBDevice_::isSuspended() {
    return (_usbSuspendState & (1 << SUSPI));
}
#endif /* if defined(USBCON) */
