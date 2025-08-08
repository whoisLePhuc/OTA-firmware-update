#include "USBAccessControlComponent/USBGuardInterface.h"
#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hàm khởi tạo struct UsbDeviceEvent từ thông tin nhận được
UsbDeviceEvent createUsbInformation(unsigned int id, unsigned int event, unsigned int target,
                                    const char* rule, const char* vendor_id, const char* product_id) {
    UsbDeviceEvent device;

    device.id = id;
    device.event = event;
    device.target = target;

    strncpy(device.rule, rule, sizeof(device.rule) - 1);
    device.rule[sizeof(device.rule) - 1] = '\0';

    strncpy(device.vendor_id, vendor_id, sizeof(device.vendor_id) - 1);
    device.vendor_id[sizeof(device.vendor_id) - 1] = '\0';

    strncpy(device.product_id, product_id, sizeof(device.product_id) - 1);
    device.product_id[sizeof(device.product_id) - 1] = '\0';

    return device;
}

// Lắng nghe signal từ USBGuard và xử lý mỗi khi có thiết bị mới
void listen_usbguard_events(void (*handler)(UsbDeviceEvent*)) {
    DBusError err;
    DBusConnection* conn;
    DBusMessage* msg;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Lỗi kết nối DBus: %s\n", err.message);
        dbus_error_free(&err);
        return;
    }
    if (!conn) {
        fprintf(stderr, "Không thể kết nối tới system bus\n");
        return;
    }

    const char* rule = "type='signal',interface='org.usbguard.Devices1',member='DevicePresenceChanged'";
    dbus_bus_add_match(conn, rule, &err);
    dbus_connection_flush(conn);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Lỗi khi thêm DBus match: %s\n", err.message);
        return;
    }

    printf("Đang lắng nghe signal từ USBGuard...\n");

    while (1) {
        dbus_connection_read_write(conn, -1);
        msg = dbus_connection_pop_message(conn);

        if (msg == NULL)
            continue;

        if (dbus_message_is_signal(msg, "org.usbguard.Devices1", "DevicePresenceChanged")) {
            DBusMessageIter iter, dictIter;
            unsigned int id, event, target;
            char* rule;
            char *vendor = NULL, *product = NULL;

            dbus_message_iter_init(msg, &iter);
            dbus_message_iter_get_basic(&iter, &id);
            dbus_message_iter_next(&iter);
            dbus_message_iter_get_basic(&iter, &event);
            dbus_message_iter_next(&iter);
            dbus_message_iter_get_basic(&iter, &target);
            dbus_message_iter_next(&iter);
            dbus_message_iter_get_basic(&iter, &rule);
            dbus_message_iter_next(&iter);

            // Đọc dictionary a{ss}
            if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
                dbus_message_iter_recurse(&iter, &dictIter);
                while (dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_DICT_ENTRY) {
                    DBusMessageIter entry;
                    char *key, *value;

                    dbus_message_iter_recurse(&dictIter, &entry);
                    dbus_message_iter_get_basic(&entry, &key);
                    dbus_message_iter_next(&entry);
                    dbus_message_iter_get_basic(&entry, &value);

                    if (strcmp(key, "idVendor") == 0) {
                        vendor = value;
                    } else if (strcmp(key, "idProduct") == 0) {
                        product = value;
                    }

                    dbus_message_iter_next(&dictIter);
                }
            }

            if (vendor && product) {
                UsbDeviceEvent device = createUsbInformation(id, event, target, rule, vendor, product);
                handler(&device); // Gọi callback xử lý
            }
        }

        dbus_message_unref(msg);
    }
}
