#include "USBAccessControlComponent/USBGuardInterface.h"
#include <stdio.h>
#include <string.h>

UsbDeviceInfo usbInfo;

DBusConnection* usbguardInitConnection(void) {
    DBusError err;
    DBusConnection* conn;

    dbus_error_init(&err);
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Kết nối lỗi: %s\n", err.message);
        dbus_error_free(&err);
    }
    if (!conn) {
        fprintf(stderr, "Không thể kết nối tới system bus\n");
        return NULL;
    }

    const char* rule = "type='signal',interface='org.usbguard.Devices1',member='DevicePresenceChanged'";
    dbus_bus_add_match(conn, rule, &err);
    dbus_connection_flush(conn);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Lỗi khi add match: %s\n", err.message);
        dbus_error_free(&err);
        return NULL;
    }

    return conn;
}

int usbguardListenEvent(DBusConnection* conn, UsbDeviceInfo* info) {
    DBusMessage* msg;
    DBusMessageIter iter, dictIter;

    dbus_connection_read_write(conn, 0);
    msg = dbus_connection_pop_message(conn);

    if (msg == NULL) {
        return 0; // Không có message
    }

    if (dbus_message_is_signal(msg, "org.usbguard.Devices1", "DevicePresenceChanged")) {
        // Reset struct trước khi lưu dữ liệu mới
        memset(info, 0, sizeof(UsbDeviceInfo));

        dbus_message_iter_init(msg, &iter);
        dbus_message_iter_get_basic(&iter, &info->id);
        dbus_message_iter_next(&iter);
        dbus_message_iter_get_basic(&iter, &info->event);
        dbus_message_iter_next(&iter);
        dbus_message_iter_get_basic(&iter, &info->target);
        dbus_message_iter_next(&iter);

        const char* rule;
        dbus_message_iter_get_basic(&iter, &rule);
        strncpy(info->device_rule, rule, sizeof(info->device_rule) - 1);
        info->device_rule[sizeof(info->device_rule) - 1] = '\0';
        dbus_message_iter_next(&iter);

        // Đọc dictionary a{ss}
        if (dbus_message_iter_get_arg_type(&iter) == DBUS_TYPE_ARRAY) {
            dbus_message_iter_recurse(&iter, &dictIter);
            int count = 0;
            while (dbus_message_iter_get_arg_type(&dictIter) == DBUS_TYPE_DICT_ENTRY &&
                   count < MAX_PROPERTIES) {
                DBusMessageIter entry;
                const char *key, *value;

                dbus_message_iter_recurse(&dictIter, &entry);
                dbus_message_iter_get_basic(&entry, &key);
                dbus_message_iter_next(&entry);
                dbus_message_iter_get_basic(&entry, &value);

                strncpy(info->properties[count].key, key, MAX_KEY_LEN - 1);
                strncpy(info->properties[count].value, value, MAX_VALUE_LEN - 1);
                count++;

                dbus_message_iter_next(&dictIter);
            }
            info->property_count = count;
        }

        dbus_message_unref(msg);
        return 1;
    }

    dbus_message_unref(msg);
    return 0;
}

void usbguardPrintInfo(const UsbDeviceInfo* info) {
    printf("USB Info:\n");
    printf("ID: %u, Event: %u, Target: %u, Rule: %s\n",
           info->id, info->event, info->target, info->device_rule);

    for (int i = 0; i < info->property_count; i++) {
        printf("  %s = %s\n", info->properties[i].key, info->properties[i].value);
    }
}

int usbguardStorageDeviceCheck(const UsbDeviceInfo* info) {
    if (info == NULL || info->property_count == 0) {
        return 0;
    }

    for (int i = 0; i < info->property_count; ++i) {
        const char* key = info->properties[i].key;
        const char* value = info->properties[i].value;

        // 1. Thiết bị chính có class = "08" (Mass Storage)
        if (strcmp(key, "deviceClass") == 0 && strcmp(value, "08") == 0) {
            return 1;
        }

        // 2. Composite device: một trong các interface có class = "08"
        if (strncmp(key, "interface.", 10) == 0 &&
            strstr(key, ".class") != NULL &&
            strcmp(value, "08") == 0) {
            return 1;
        }

        // 3. Tên chứa "Storage"
        if (strcmp(key, "name") == 0 &&
            strstr(value, "Storage") != NULL) {
            return 1;
        }

        // 4. with-interface = "08:06:50" hoặc bắt đầu bằng "08:"
        if (strcmp(key, "with-interface") == 0 &&
            (strcmp(value, "08:06:50") == 0 ||
             strncmp(value, "08:", 3) == 0)) {
            return 1;
        }
    }

    return 0;
}
