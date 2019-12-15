#ifndef __USBAPI__
#define __USBAPI__
#include <inttypes.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
#include "Arduino.h"
//如果要将EP_SIZE减小到16，此定义很有用
//目前，除控制端点外，所有EP仅支持64和16作为EP_SIZE
#ifndef USB_EP_SIZE
#define USB_EP_SIZE 64
#endif
#if defined(USBCON)
#include "USBDesc.h"
#include "USBCore.h"
// USB用到的常量
#define EP_TYPE_CONTROL             (0x00)
#define EP_TYPE_BULK_IN             ((1<<EPTYPE1) | (1<<EPDIR))
#define EP_TYPE_BULK_OUT            (1<<EPTYPE1)
#define EP_TYPE_INTERRUPT_IN        ((1<<EPTYPE1) | (1<<EPTYPE0) | (1<<EPDIR))
#define EP_TYPE_INTERRUPT_OUT       ((1<<EPTYPE1) | (1<<EPTYPE0))
#define EP_TYPE_ISOCHRONOUS_IN      ((1<<EPTYPE0) | (1<<EPDIR))
#define EP_TYPE_ISOCHRONOUS_OUT     (1<<EPTYPE0)
class USBDevice_ {
public:
    USBDevice_();
    bool configured();
    void attach();
    void detach();  // 串口掉线
    void poll();
    bool wakeupHost(); // 如果无法处理唤醒，则返回false
    bool isSuspended();
};
extern USBDevice_ USBDevice;
// 通过CDC串行（Serial1是物理端口）
struct ring_buffer;
#ifndef SERIAL_BUFFER_SIZE
#if ((RAMEND - RAMSTART) < 1023)
#define SERIAL_BUFFER_SIZE 16
#else
#define SERIAL_BUFFER_SIZE 64
#endif
#endif
#if (SERIAL_BUFFER_SIZE>256)
#error Please lower the CDC Buffer size
#endif
class Serial_ : public Stream {
private:
    int peek_buffer;
public:
    Serial_() {
        peek_buffer = -1;
    };
    void begin(unsigned long);
    void begin(unsigned long, uint8_t);
    void end(void);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    using Print::write; // 从打印中提取write(str)和write(buf，size）
    operator bool();
    volatile uint8_t _rx_buffer_head;
    volatile uint8_t _rx_buffer_tail;
    unsigned char _rx_buffer[SERIAL_BUFFER_SIZE];
    // 此方法允许处理由发送的“ SEND_BREAK”请求到USB主机。 这些请求表明主机希望发送BREAK信号，并附带一个uint16_t值用于指定休息时间。
    // 值0表示结束当前的中断，而值0xffff表示开始无限期休息。
    // readBreak()将返回最新中断的值请求，但最多返回一次，当返回时返回-1
    // readBreak()再次被调用（直到另一个中断请求收到，再次返回一次）。
    // 这也意味着如果收到两个中断请求，在不调用readBreak（）的情况下，第一个请求丢失。
    // 返回的值很长，因此可以返回0-0xffff以及-1。
    int32_t readBreak();
    // 这些将返回USB主机为串行端口。这些并没有真正使用，但是在这里提供如果草图要作用于这些设置。
    uint32_t baud();
    uint8_t stopbits();
    uint8_t paritytype();
    uint8_t numbits();
    bool dtr();
    bool rts();
    enum {
        ONE_STOP_BIT = 0,
        ONE_AND_HALF_STOP_BIT = 1,
        TWO_STOP_BITS = 2,
    };
    enum {
        NO_PARITY = 0,
        ODD_PARITY = 1,
        EVEN_PARITY = 2,
        MARK_PARITY = 3,
        SPACE_PARITY = 4,
    };

};
extern Serial_ Serial;
#define HAVE_CDCSERIAL
// 底层API
typedef struct {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint8_t wValueL;
    uint8_t wValueH;
    uint16_t wIndex;
    uint16_t wLength;
} USBSetup;
// MSC驱动
int     MSC_GetInterface(uint8_t *interfaceNum);
int     MSC_GetDescriptor(int i);
bool    MSC_Setup(USBSetup &setup);
bool    MSC_Data(uint8_t rx, uint8_t tx);
// CSC驱动
int     CDC_GetInterface(uint8_t *interfaceNum);
int     CDC_GetDescriptor(int i);
bool    CDC_Setup(USBSetup &setup);
#define TRANSFER_PGM        0x80
#define TRANSFER_RELEASE    0x40
#define TRANSFER_ZERO       0x20
int USB_SendControl(uint8_t flags, const void *d, int len);
int USB_RecvControl(void *d, int len);
int USB_RecvControlLong(void *d, int len);
uint8_t USB_Available(uint8_t ep);
uint8_t USB_SendSpace(uint8_t ep);
int USB_Send(uint8_t ep, const void *data, int len);    // blocking
int USB_Recv(uint8_t ep, void *data, int len);      // non-blocking
int USB_Recv(uint8_t ep);                           // non-blocking
void USB_Flush(uint8_t ep);
#endif

#endif /* if defined(USBCON) */
