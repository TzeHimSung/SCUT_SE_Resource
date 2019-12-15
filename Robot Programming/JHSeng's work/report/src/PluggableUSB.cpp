#include "USBAPI.h"
#include "PluggableUSB.h"
#if defined(USBCON)
#ifdef PLUGGABLE_USB_ENABLED

extern uint8_t _initEndpoints[];

int PluggableUSB_::getInterface(uint8_t *interfaceCount) {
    int sent = 0;
    PluggableUSBModule *node;
    for (node = rootNode; node; node = node->next) {
        int res = node->getInterface(interfaceCount);
        if (res < 0)
            return -1;
        sent += res;
    }
    return sent;
}

int PluggableUSB_::getDescriptor(USBSetup &setup) {
    PluggableUSBModule *node;
    for (node = rootNode; node; node = node->next) {
        int ret = node->getDescriptor(setup);
        // ret!=0 -> request has been processed
        if (ret)
            return ret;
    }
    return 0;
}

void PluggableUSB_::getShortName(char *iSerialNum) {
    PluggableUSBModule *node;
    for (node = rootNode; node; node = node->next) {
        iSerialNum += node->getShortName(iSerialNum);
    }
    *iSerialNum = 0;
}

bool PluggableUSB_::setup(USBSetup &setup) {
    PluggableUSBModule *node;
    for (node = rootNode; node; node = node->next) {
        if (node->setup(setup)) {
            return true;
        }
    }
    return false;
}

bool PluggableUSB_::plug(PluggableUSBModule *node) {
    if ((lastEp + node->numEndpoints) > USB_ENDPOINTS) {
        return false;
    }
    if (!rootNode) rootNode = node;
    else {
        PluggableUSBModule *current = rootNode;
        while (current->next) current = current->next;
        current->next = node;
    }
    node->pluggedInterface = lastIf;
    node->pluggedEndpoint = lastEp;
    lastIf += node->numInterfaces;
    for (uint8_t i = 0; i < node->numEndpoints; i++) {
        _initEndpoints[lastEp] = node->endpointType[i];
        lastEp++;
    }
    return true;
}

PluggableUSB_ &PluggableUSB() {
    static PluggableUSB_ obj;
    return obj;
}

PluggableUSB_::PluggableUSB_() : lastIf(CDC_ACM_INTERFACE + CDC_INTERFACE_COUNT),
    lastEp(CDC_FIRST_ENDPOINT + CDC_ENPOINT_COUNT),
    rootNode(NULL) {
    // 无
}

#endif

#endif /* if defined(USBCON) */
