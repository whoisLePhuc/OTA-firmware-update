#ifndef USB_GUARD_INTERFACE_H
#define USB_GUARD_INTERFACE_H

#include <dbus/dbus.h>

#define MAX_RULE_LEN 256
#define MAX_KEY_LEN 64
#define MAX_VALUE_LEN 128
#define MAX_PROPERTIES 20  // Giới hạn số lượng thuộc tính dictionary

// Struct lưu một cặp key-value trong dictionary
typedef struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
} UsbProperty;

// Struct lưu toàn bộ thông tin thiết bị USB
typedef struct {
    unsigned int id;
    unsigned int event;
    unsigned int target;
    char device_rule[MAX_RULE_LEN];
    UsbProperty properties[MAX_PROPERTIES];
    int property_count;
} UsbDeviceInfo;

// API khởi tạo kết nối DBus
DBusConnection* usbguardInitConnection(void);

// API lắng nghe sự kiện USB và lưu vào struct
int usbguardListenEvent(DBusConnection* conn, UsbDeviceInfo* info);

// API in thông tin USB ra console
void usbguardPrintInfo(const UsbDeviceInfo* info);

// Hàm kiểm tra thiết bị storage
int usbguardStorageDeviceCheck(const UsbDeviceInfo* info);

// Biến toàn cầu để lưu trữ thông tin của USB
extern UsbDeviceInfo usbInfo;

#endif
