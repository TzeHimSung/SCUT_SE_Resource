#ifndef udp_h
#define udp_h

#include <Stream.h>
#include <IPAddress.h>

class UDP : public Stream {

public:
    virtual uint8_t begin(uint16_t) = 0;
    virtual uint8_t beginMulticast(IPAddress, uint16_t) {
        return 0;    // 初始化，开始在指定IP和端口上侦听。 如果成功，则返回1；如果没有可用的套接字，则返回0
    }
    virtual void stop() = 0; // 完成UDP套接字

    // 发送UDP数据包

    // 开始建立数据包以发送到特定于ip和port的远程主机
    // 如果成功，则返回1，如果提供的IP地址或端口有问题，则返回0
    virtual int beginPacket(IPAddress ip, uint16_t port) = 0;
    // 开始建立数据包以发送到特定于ip和port的远程主机
    // 如果成功，则返回1，如果提供的IP地址或端口有问题，则返回0
    virtual int beginPacket(const char *host, uint16_t port) = 0;
    // 完成此数据包并发送。如果成功发送数据包，则返回1；如果发生错误，则返回0
    virtual int endPacket() = 0;
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t *buffer, size_t size) = 0;
    // 开始处理下一个可用的传入数据包
    virtual int parsePacket() = 0;
    // 当前数据包中剩余的字节数
    virtual int available() = 0;
    // 从当前数据包中读取一个字节
    virtual int read() = 0;
    // 从当前数据包中读取最多len个字节并将其放入缓冲区
    // 返回读取的字节数；如果没有可用字节，则返回0
    virtual int read(unsigned char *buffer, size_t len) = 0;
    // 从当前数据包中读取最多len个字符并将其放入缓冲区
    // 返回读取的字符数，如果没有可用字符，则返回0
    virtual int read(char *buffer, size_t len) = 0;
    // 返回当前数据包的下一个字节，而不继续下一个字节
    virtual int peek() = 0;
    virtual void flush() = 0; // 完成读取当前数据包
    // 返回发送当前传入数据包的主机的IP地址
    virtual IPAddress remoteIP() = 0;
    // 返回发送当前传入数据包的主机的端口
    virtual uint16_t remotePort() = 0;
protected:
    uint8_t *rawIPAddress(IPAddress &addr) {
        return addr.raw_address();
    };
};

#endif
