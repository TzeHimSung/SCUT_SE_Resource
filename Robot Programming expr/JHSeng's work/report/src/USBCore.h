#ifndef __USBCORE_H__
#define __USBCORE_H__
#include "USBAPI.h"
// 定义标准请求
#define GET_STATUS          0
#define CLEAR_FEATURE       1
#define SET_FEATURE         3
#define SET_ADDRESS         5
#define GET_DESCRIPTOR      6
#define SET_DESCRIPTOR      7
#define GET_CONFIGURATION   8
#define SET_CONFIGURATION   9
#define GET_INTERFACE       10
#define SET_INTERFACE       11
// bm请求类型
#define REQUEST_HOSTTODEVICE    0x00
#define REQUEST_DEVICETOHOST    0x80
#define REQUEST_DIRECTION       0x80
#define REQUEST_STANDARD        0x00
#define REQUEST_CLASS           0x20
#define REQUEST_VENDOR          0x40
#define REQUEST_TYPE            0x60
#define REQUEST_DEVICE          0x00
#define REQUEST_INTERFACE       0x01
#define REQUEST_ENDPOINT        0x02
#define REQUEST_OTHER           0x03
#define REQUEST_RECIPIENT       0x03
#define REQUEST_DEVICETOHOST_CLASS_INTERFACE    (REQUEST_DEVICETOHOST | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_HOSTTODEVICE_CLASS_INTERFACE    (REQUEST_HOSTTODEVICE | REQUEST_CLASS | REQUEST_INTERFACE)
#define REQUEST_DEVICETOHOST_STANDARD_INTERFACE (REQUEST_DEVICETOHOST | REQUEST_STANDARD | REQUEST_INTERFACE)
// 类请求
#define CDC_SET_LINE_CODING         0x20
#define CDC_GET_LINE_CODING         0x21
#define CDC_SET_CONTROL_LINE_STATE  0x22
#define CDC_SEND_BREAK              0x23
#define MSC_RESET                   0xFF
#define MSC_GET_MAX_LUN             0xFE
// 描述器
#define USB_DEVICE_DESC_SIZE 18
#define USB_CONFIGUARTION_DESC_SIZE 9
#define USB_INTERFACE_DESC_SIZE 9
#define USB_ENDPOINT_DESC_SIZE 7
#define USB_DEVICE_DESCRIPTOR_TYPE             1
#define USB_CONFIGURATION_DESCRIPTOR_TYPE      2
#define USB_STRING_DESCRIPTOR_TYPE             3
#define USB_INTERFACE_DESCRIPTOR_TYPE          4
#define USB_ENDPOINT_DESCRIPTOR_TYPE           5
// 标准feature
#define DEVICE_REMOTE_WAKEUP                   1
#define ENDPOINT_HALT                          2
#define TEST_MODE                              3
// GetStatus()请求返回给设备的信息
#define FEATURE_SELFPOWERED_ENABLED     (1 << 0)
#define FEATURE_REMOTE_WAKEUP_ENABLED   (1 << 1)
#define USB_DEVICE_CLASS_COMMUNICATIONS        0x02
#define USB_DEVICE_CLASS_HUMAN_INTERFACE       0x03
#define USB_DEVICE_CLASS_STORAGE               0x08
#define USB_DEVICE_CLASS_VENDOR_SPECIFIC       0xFF
#define USB_CONFIG_POWERED_MASK                0x40
#define USB_CONFIG_BUS_POWERED                 0x80
#define USB_CONFIG_SELF_POWERED                0xC0
#define USB_CONFIG_REMOTE_WAKEUP               0x20
// 设置bMaxPower
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)
// 端点描述符中的端点地址
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 (lowByte((addr) | 0x00))
#define USB_ENDPOINT_IN(addr)                  (lowByte((addr) | 0x80))
#define USB_ENDPOINT_TYPE_MASK                 0x03
#define USB_ENDPOINT_TYPE_CONTROL              0x00
#define USB_ENDPOINT_TYPE_ISOCHRONOUS          0x01
#define USB_ENDPOINT_TYPE_BULK                 0x02
#define USB_ENDPOINT_TYPE_INTERRUPT            0x03
#define TOBYTES(x) ((x) & 0xFF),(((x) >> 8) & 0xFF)
#define CDC_V1_10                               0x0110
#define CDC_COMMUNICATION_INTERFACE_CLASS       0x02
#define CDC_CALL_MANAGEMENT                     0x01
#define CDC_ABSTRACT_CONTROL_MODEL              0x02
#define CDC_HEADER                              0x00
#define CDC_ABSTRACT_CONTROL_MANAGEMENT         0x02
#define CDC_UNION                               0x06
#define CDC_CS_INTERFACE                        0x24
#define CDC_CS_ENDPOINT                         0x25
#define CDC_DATA_INTERFACE_CLASS                0x0A
#define MSC_SUBCLASS_SCSI                       0x06
#define MSC_PROTOCOL_BULK_ONLY                  0x50
#ifndef USB_VERSION
#define USB_VERSION 0x200
#endif
// 设备
typedef struct {
    u8 len;             // 18
    u8 dtype;           // 1 USB设备描述类型
    u16 usbVersion;     // 0x200 或 0x210
    u8  deviceClass;
    u8  deviceSubClass;
    u8  deviceProtocol;
    u8  packetSize0;    // Packet 0
    u16 idVendor;
    u16 idProduct;
    u16 deviceVersion;  // 0x100
    u8  iManufacturer;
    u8  iProduct;
    u8  iSerialNumber;
    u8  bNumConfigurations;
} DeviceDescriptor;
// 设置
typedef struct {
    u8  len;            // 9
    u8  dtype;          // 2
    u16 clen;           // 总长度
    u8  numInterfaces;
    u8  config;
    u8  iconfig;
    u8  attributes;
    u8  maxPower;
} ConfigDescriptor;
// 字符串接口
typedef struct {
    u8 len;     // 9
    u8 dtype;   // 4
    u8 number;
    u8 alternate;
    u8 numEndpoints;
    u8 interfaceClass;
    u8 interfaceSubClass;
    u8 protocol;
    u8 iInterface;
} InterfaceDescriptor;
//  字符串端点
typedef struct {
    u8 len;     // 7
    u8 dtype;   // 5
    u8 addr;
    u8 attr;
    u16 packetSize;
    u8 interval;
} EndpointDescriptor;
// 接口关联描述符，用于将2个接口绑定到CDC复合设备中
typedef struct {
    u8 len;             // 8
    u8 dtype;           // 11
    u8 firstInterface;
    u8 interfaceCount;
    u8 functionClass;
    u8 funtionSubClass;
    u8 functionProtocol;
    u8 iInterface;
} IADDescriptor;
// CDC CS接口描述符
typedef struct {
    u8 len;     // 5
    u8 dtype;   // 0x24
    u8 subtype;
    u8 d0;
    u8 d1;
} CDCCSInterfaceDescriptor;
typedef struct {
    u8 len;     // 4
    u8 dtype;   // 0x24
    u8 subtype;
    u8 d0;
} CDCCSInterfaceDescriptor4;
typedef struct {
    u8  len;
    u8  dtype;      // 0x24
    u8  subtype;    // 1
    u8  bmCapabilities;
    u8  bDataInterface;
} CMFunctionalDescriptor;
typedef struct {
    u8  len;
    u8  dtype;      // 0x24
    u8  subtype;    // 1
    u8  bmCapabilities;
} ACMFunctionalDescriptor;
typedef struct {
    //  IAD
    IADDescriptor               iad;    // 仅在复合设备上需要
    //  控制
    InterfaceDescriptor         cif;
    CDCCSInterfaceDescriptor    header;
    CMFunctionalDescriptor      callManagement;         // 调用管理
    ACMFunctionalDescriptor     controlManagement;      // ACM
    CDCCSInterfaceDescriptor    functionalDescriptor;   // CDC单元
    EndpointDescriptor          cifin;
    //  数据
    InterfaceDescriptor         dif;
    EndpointDescriptor          in;
    EndpointDescriptor          out;
} CDCDescriptor;
typedef struct {
    InterfaceDescriptor         msc;
    EndpointDescriptor          in;
    EndpointDescriptor          out;
} MSCDescriptor;
#define D_DEVICE(_class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs) \
    { 18, 1, USB_VERSION, _class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs }
#define D_CONFIG(_totalLength,_interfaces) \
    { 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_BUS_POWERED | USB_CONFIG_REMOTE_WAKEUP, USB_CONFIG_POWER_MA(500) }
#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
    { 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }
#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
    { 7, 5, _addr,_attr,_packetSize, _interval }
#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
    { 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }
#define D_CDCCS(_subtype,_d0,_d1)   { 5, 0x24, _subtype, _d0, _d1 }
#define D_CDCCS4(_subtype,_d0)      { 4, 0x24, _subtype, _d0 }
// Bootloader相关字段旧的Caterina引导程序将MAGIC密钥放置在不安全的RAM位置（可以将其重写，由实际运行之前的运行草图开始）。
// 闪存末尾的LUFA“签名”可以识别较新的引导加载程序，usafe和安全位置。
#ifndef MAGIC_KEY
#define MAGIC_KEY 0x7777
#endif
#ifndef MAGIC_KEY_POS
#define MAGIC_KEY_POS 0x0800
#endif
#ifndef NEW_LUFA_SIGNATURE
#define NEW_LUFA_SIGNATURE 0xDCFB
#endif
#endif