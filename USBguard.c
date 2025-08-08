#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    unsigned int id;
    unsigned int event;
    unsigned int target;
    char device_rule[512];
} USBDeviceInfo;

typedef void (*USBSignalCallback)(USBDeviceInfo info);

// Hàm kết nối D-Bus
DBusConnection* connect_dbus() {
    DBusError err;
    dbus_error_init(&err);
    DBusConnection* conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "DBus error: %s\n", err.message);
        dbus_error_free(&err);
    }
    return conn;
}

// Hàm xử lý signal USB và gọi callback
void handle_signal(DBusMessage *msg, USBSignalCallback callback) {
    DBusMessageIter iter;
    unsigned int id, event, target;
    char* device_rule;

    dbus_message_iter_init(msg, &iter);
    dbus_message_iter_get_basic(&iter, &id);
    dbus_message_iter_next(&iter);
    dbus_message_iter_get_basic(&iter, &event);
    dbus_message_iter_next(&iter);
    dbus_message_iter_get_basic(&iter, &target);
    dbus_message_iter_next(&iter);
    dbus_message_iter_get_basic(&iter, &device_rule);

    USBDeviceInfo info = { id, event, target, "" };
    strncpy(info.device_rule, device_rule, sizeof(info.device_rule) - 1);

    if (callback) {
        callback(info);
    }
}

// Lắng nghe tín hiệu và truyền về hàm callback
void listen_for_usb_signals(USBSignalCallback callback) {
    DBusConnection* conn = connect_dbus();
    DBusMessage* msg;

    if (!conn) return;

    dbus_bus_add_match(conn,
        "type='signal',interface='org.usbguard.Devices1',member='DevicePresenceChanged'",
        NULL);
    dbus_connection_flush(conn);

    printf("Listening for USBGuard signals...\n");

    while (1) {
        dbus_connection_read_write(conn, -1);
        msg = dbus_connection_pop_message(conn);
        if (msg == NULL) continue;

        if (dbus_message_is_signal(msg, "org.usbguard.Devices1", "DevicePresenceChanged")) {
            handle_signal(msg, callback);
        }

        dbus_message_unref(msg);
    }
}


void usb_event_handler(USBDeviceInfo info) {
    printf("📥 USB Signal Received:\n");
    printf("ID: %u, Event: %u, Target: %u, Rule: %s\n",
           info.id, info.event, info.target, info.device_rule);

    // TODO: gọi hàm check_whitelist(info)
    // TODO: ghi ra file csv
    // TODO: gửi tín hiệu cho module khác nếu cần
}

int main() {
    listen_for_usb_signals(usb_event_handler);
    return 0;
}
