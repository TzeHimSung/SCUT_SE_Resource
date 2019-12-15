#include <Arduino.h>

// 直接返回
int atexit(void ( * /*func*/ )()) {
    return 0;
}

// 在Arduino.h中声明为弱允许用户重新定义。
void initVariant() __attribute__((weak));
void initVariant() { }
// USB设置
void setupUSB() __attribute__((weak));
void setupUSB() { }

int main(void) {
    init();
    initVariant();
#if defined(USBCON)
    USBDevice.attach();
#endif
    // 运行用户所定义的初始化行为
    setup();
    for (;;) {
        // 运行用户所定义的循环行为
        loop();
        // 执行串口通信
        if (serialEventRun) serialEventRun();
    }
    return 0;
}