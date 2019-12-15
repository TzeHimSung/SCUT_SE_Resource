#include "USBAPI.h"
#include <avr/wdt.h>
#include <util/atomic.h>

#if defined(USBCON)

typedef struct {
    u32 dwDTERate;
    u8  bCharFormat;
    u8  bParityType;
    u8  bDataBits;
    u8  lineState;
} LineInfo;

static volatile LineInfo _usbLineInfo = { 57600, 0x00, 0x00, 0x00, 0x00 };
static volatile int32_t breakValue = -1;

static u8 wdtcsr_save;

#define WEAK __attribute__ ((weak))

extern const CDCDescriptor _cdcInterface PROGMEM;
const CDCDescriptor _cdcInterface = {
    D_IAD(0, 2, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 1),

    // CDC通讯接口
    D_INTERFACE(CDC_ACM_INTERFACE, 1, CDC_COMMUNICATION_INTERFACE_CLASS, CDC_ABSTRACT_CONTROL_MODEL, 0),
    D_CDCCS(CDC_HEADER, 0x10, 0x01),
    D_CDCCS(CDC_CALL_MANAGEMENT, 1, 1),                         // 设备处理呼叫管理
    D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT, 6),               // 支持SET_LINE_CODING，GET_LINE_CODING，SET_CONTROL_LINE_STATE
    D_CDCCS(CDC_UNION, CDC_ACM_INTERFACE, CDC_DATA_INTERFACE),  // 通讯接口为主机，数据接口为从机0
    D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_ACM), USB_ENDPOINT_TYPE_INTERRUPT, 0x10, 0x40),
    // CDC数据接口
    D_INTERFACE(CDC_DATA_INTERFACE, 2, CDC_DATA_INTERFACE_CLASS, 0, 0),
    D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0),
    D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_IN ), USB_ENDPOINT_TYPE_BULK, USB_EP_SIZE, 0)
};

bool isLUFAbootloader() {
    return pgm_read_word(FLASHEND - 1) == NEW_LUFA_SIGNATURE;
}

int CDC_GetInterface(u8 *interfaceNum) {
    interfaceNum[0] += 2;
    return USB_SendControl(TRANSFER_PGM, &_cdcInterface, sizeof(_cdcInterface));
}

bool CDC_Setup(USBSetup &setup) {
    u8 r = setup.bRequest;
    u8 requestType = setup.bmRequestType;
    if (REQUEST_DEVICETOHOST_CLASS_INTERFACE == requestType) {
        if (CDC_GET_LINE_CODING == r) {
            USB_SendControl(0, (void *)&_usbLineInfo, 7);
            return true;
        }
    }

    if (REQUEST_HOSTTODEVICE_CLASS_INTERFACE == requestType) {
        if (CDC_SEND_BREAK == r) {
            breakValue = ((uint16_t)setup.wValueH << 8) | setup.wValueL;
        }
        if (CDC_SET_LINE_CODING == r) {
            USB_RecvControl((void *)&_usbLineInfo, 7);
        }
        if (CDC_SET_CONTROL_LINE_STATE == r) {
            _usbLineInfo.lineState = setup.wValueL;
            // 当以1200 bps的速度打开的端口关闭时，会触发自动重置到引导加载程序中。
            // 这是较长时间启动看门狗的信号，因此它可以完成内部管理任务，例如在草图结束之前为端点提供服务
            uint16_t magic_key_pos = MAGIC_KEY_POS;

// 如果我们不直接使用新的RAMEND，请手动检查是否有更新的引导程序。
// 这是用来与旧的leonardo引导程序兼容。
// 您仍然可以将魔术键的位置手动设置为RAMEND-1，以节省一些字节用于此检查。
#if MAGIC_KEY_POS != (RAMEND-1)
            // 对于将来的板，请将密钥保存在无问题的RAMEND中
            // 这是为main()返回值保留的()永远不会返回
            if (isLUFAbootloader()) {
                // 新的引导程序
                magic_key_pos = (RAMEND - 1);
            }
#endif
            // 我们检查DTR状态，以确定主机端口是否打开(lineState的位0)。
            if (1200 == _usbLineInfo.dwDTERate && (_usbLineInfo.lineState & 0x01) == 0) {
#if MAGIC_KEY_POS != (RAMEND-1)
                // 备份ram值，如果它不是较新的引导程序，并且尚未保存。
                // 这应该至少避免一点(不是完全)破坏内存
                if (magic_key_pos != (RAMEND - 1) && *(uint16_t *)magic_key_pos != MAGIC_KEY) {
                    *(uint16_t *)(RAMEND - 1) = *(uint16_t *)magic_key_pos;
                }
#endif
                *(uint16_t *)magic_key_pos = MAGIC_KEY;
                // 保存看门狗状态，以防复位中止。
                wdtcsr_save = WDTCSR;
                wdt_enable(WDTO_120MS);
            } else if (*(uint16_t *)magic_key_pos == MAGIC_KEY) {
                // 在配置端口时，大多数操作系统都会执行一些中间步骤，并且DTR在稳定之前可能会多次摆动。
                // 为了避免虚假复位，我们将看门狗设置为120ms，如果DTR回到高电平，则最终将其取消。
                // 仅当启动了自动重置时才执行取消，这由设置的魔术键指示。
                wdt_reset();
                // 如果草图正在使用它，请还原看门狗状态。
                WDTCSR |= (1 << WDCE) | (1 << WDE);
                WDTCSR = wdtcsr_save;
#if MAGIC_KEY_POS != (RAMEND-1)
                // 恢复备份的(旧的引导程序)魔术密钥数据
                if (magic_key_pos != (RAMEND - 1)) {
                    *(uint16_t *)magic_key_pos = *(uint16_t *)(RAMEND - 1);
                } else
#endif
                {
                    *(uint16_t *)magic_key_pos = 0x0000;
                }
            }
        }
        return true;
    }
    return false;
}


void Serial_::begin(unsigned long /* baud_count */) {
    peek_buffer = -1;
}

void Serial_::begin(unsigned long /* baud_count */, byte /* config */) {
    peek_buffer = -1;
}

void Serial_::end(void) {
}

int Serial_::available(void) {
    if (peek_buffer >= 0) {
        return 1 + USB_Available(CDC_RX);
    }
    return USB_Available(CDC_RX);
}

int Serial_::peek(void) {
    if (peek_buffer < 0)
        peek_buffer = USB_Recv(CDC_RX);
    return peek_buffer;
}

int Serial_::read(void) {
    if (peek_buffer >= 0) {
        int c = peek_buffer;
        peek_buffer = -1;
        return c;
    }
    return USB_Recv(CDC_RX);
}

int Serial_::availableForWrite(void) {
    return USB_SendSpace(CDC_TX);
}

void Serial_::flush(void) {
    USB_Flush(CDC_TX);
}

size_t Serial_::write(uint8_t c) {
    return write(&c, 1);
}

size_t Serial_::write(const uint8_t *buffer, size_t size) {
    // 仅在高级CDC连接本身已打开(而不仅仅是管道)打开的情况下，才尝试发送字节-操作系统应在打开端口时设置lineState，并在关闭端口时清除lineState。
    // 在用户打开连接之前或在连接关闭之后发送的字节都会丢失-就像使用UART一样。
    if (_usbLineInfo.lineState > 0) {
        int r = USB_Send(CDC_TX, buffer, size);
        if (r > 0) {
            return r;
        } else {
            setWriteError();
            return 0;
        }
    }
    setWriteError();
    return 0;
}

// 此操作程序是草图检查端口是否已由主机实际配置和打开的便捷方式(与仅连接到主机相反)。 例如，可以在打印前在setup()中使用它，以确保主机上的应用程序实际上已准备好接收和显示数据。
// 在返回之前，我们修复了一个短暂的延迟，以修复由Federico观察到的错误，该错误已配置端口(lineState！= 0)但未完全打开。
Serial_::operator bool() {
    bool result = false;
    if (_usbLineInfo.lineState > 0)
        result = true;
    delay(10);
    return result;
}

unsigned long Serial_::baud() {
    uint32_t baudrate;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        baudrate =  _usbLineInfo.dwDTERate;
    }
    return baudrate;
}

uint8_t Serial_::stopbits() {
    return _usbLineInfo.bCharFormat;
}

uint8_t Serial_::paritytype() {
    return _usbLineInfo.bParityType;
}

uint8_t Serial_::numbits() {
    return _usbLineInfo.bDataBits;
}

bool Serial_::dtr() {
    return _usbLineInfo.lineState & 0x1;
}

bool Serial_::rts() {
    return _usbLineInfo.lineState & 0x2;
}

int32_t Serial_::readBreak() {
    int32_t ret;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        ret = breakValue;
        breakValue = -1;
    }
    return ret;
}

Serial_ Serial;

#endif /* if defined(USBCON) */
