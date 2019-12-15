#ifndef IPAddress_h
#define IPAddress_h

#include <stdint.h>
#include "Printable.h"
#include "WString.h"

// 简化IP地址处理和传递的类

class IPAddress : public Printable {
private:
    union {
        uint8_t bytes[4];  // IPv4地址
        uint32_t dword;
    } _address;

    // 访问包含地址的原始字节数组。 因为此操作返回的是指向内部结构的指针，而不是地址的副本，所以仅当您知道返回的uint8_t *的用法将是瞬态的且不存储时，才应使用此函数。
    uint8_t *raw_address() {
        return _address.bytes;
    };

public:
    // 构造函数
    IPAddress();
    IPAddress(uint8_t first_octet, uint8_t second_octet, uint8_t third_octet, uint8_t fourth_octet);
    IPAddress(uint32_t address);
    IPAddress(const uint8_t *address);
    bool fromString(const char *address);
    bool fromString(const String &address) {
        return fromString(address.c_str());
    }
    // 重载的强制转换运算符，允许在预期指向四字节uint8_t数组的指针的地方使用IPAddress对象
    operator uint32_t() const {
        return _address.dword;
    };
    bool operator==(const IPAddress &addr) const {
        return _address.dword == addr._address.dword;
    };
    bool operator==(const uint8_t *addr) const;

    // 重载索引运算符，以允许获取和设置地址的各个八位字节
    uint8_t operator[](int index) const {
        return _address.bytes[index];
    };
    uint8_t &operator[](int index) {
        return _address.bytes[index];
    };

    // 重载了复制运算符，以允许初始化其他类型的IPAddress对象
    IPAddress &operator=(const uint8_t *address);
    IPAddress &operator=(uint32_t address);
    virtual size_t printTo(Print &p) const;
    friend class EthernetClass;
    friend class UDP;
    friend class Client;
    friend class Server;
    friend class DhcpClass;
    friend class DNSClient;
};

const IPAddress INADDR_NONE(0, 0, 0, 0);

#endif
