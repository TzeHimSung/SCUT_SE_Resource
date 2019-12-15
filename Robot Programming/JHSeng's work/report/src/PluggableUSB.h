#ifndef PUSB_h
#define PUSB_h

#include "USBAPI.h"
#include <stdint.h>

#if defined(USBCON)

class PluggableUSBModule {
public:
    PluggableUSBModule(uint8_t numEps, uint8_t numIfs, uint8_t *epType) :
        numEndpoints(numEps), numInterfaces(numIfs), endpointType(epType)
    { }

protected:
    virtual bool setup(USBSetup &setup) = 0;
    virtual int getInterface(uint8_t *interfaceCount) = 0;
    virtual int getDescriptor(USBSetup &setup) = 0;
    virtual uint8_t getShortName(char *name) {
        name[0] = 'A' + pluggedInterface;
        return 1;
    }

    uint8_t pluggedInterface;
    uint8_t pluggedEndpoint;

    const uint8_t numEndpoints;
    const uint8_t numInterfaces;
    const uint8_t *endpointType;

    PluggableUSBModule *next = NULL;

    friend class PluggableUSB_;
};

class PluggableUSB_ {
public:
    PluggableUSB_();
    bool plug(PluggableUSBModule *node);
    int getInterface(uint8_t *interfaceCount);
    int getDescriptor(USBSetup &setup);
    bool setup(USBSetup &setup);
    void getShortName(char *iSerialNum);

private:
    uint8_t lastIf;
    uint8_t lastEp;
    PluggableUSBModule *rootNode;
};
// 替换为全局单例。
// 此功能可防止静态初始化订单失败
PluggableUSB_ &PluggableUSB();
#endif
#endif
