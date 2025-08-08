#include "USBAccessControlComponent/UdevListener.h"
#include "EventManager/EventManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <time.h>

#define UEVENT_BUFFER_SIZE 2048
#define CSV_FILE "/home/lephuc/OTA-firmware-update/csv_file/ConnectDevice.csv"

static int sock_fd = -1;

static char *get_value(char *buf, int len, const char *key) {
    char *ptr = buf;
    char *end = buf + len;
    size_t key_len = strlen(key);

    while (ptr < end && *ptr) {
        if (strncmp(ptr, key, key_len) == 0) {
            return ptr + key_len;
        }
        ptr += strlen(ptr) + 1;
    }
    return NULL;
}

static void get_timestamp(char *buf, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", t);
}

static void log_usb_event(const char *action, char *buf, int len) {
    char timestamp[64];
    get_timestamp(timestamp, sizeof(timestamp));

    char *devpath = get_value(buf, len, "DEVPATH=");

    char *vendor_id = NULL;
    char *product_id = NULL;
    char *vendor = NULL;
    char *model = NULL;

    // Chỉ đọc chi tiết khi INSERTED
    if (strcmp(action, "INSERTED") == 0 && devpath) {
        char sys_vendor[64], sys_product[64], sys_manufacturer[64], sys_model[64];
        char path[256];

        snprintf(path, sizeof(path), "/sys%s/idVendor", devpath);
        FILE *fp = fopen(path, "r");
        if (fp && fgets(sys_vendor, sizeof(sys_vendor), fp)) {
            sys_vendor[strcspn(sys_vendor, "\n")] = 0;
            vendor_id = sys_vendor;
        }
        if (fp) fclose(fp);

        snprintf(path, sizeof(path), "/sys%s/idProduct", devpath);
        fp = fopen(path, "r");
        if (fp && fgets(sys_product, sizeof(sys_product), fp)) {
            sys_product[strcspn(sys_product, "\n")] = 0;
            product_id = sys_product;
        }
        if (fp) fclose(fp);

        snprintf(path, sizeof(path), "/sys%s/manufacturer", devpath);
        fp = fopen(path, "r");
        if (fp && fgets(sys_manufacturer, sizeof(sys_manufacturer), fp)) {
            sys_manufacturer[strcspn(sys_manufacturer, "\n")] = 0;
            vendor = sys_manufacturer;
        }
        if (fp) fclose(fp);

        snprintf(path, sizeof(path), "/sys%s/product", devpath);
        fp = fopen(path, "r");
        if (fp && fgets(sys_model, sizeof(sys_model), fp)) {
            sys_model[strcspn(sys_model, "\n")] = 0;
            model = sys_model;
        }
        if (fp) fclose(fp);
    }

    FILE *fp = fopen(CSV_FILE, "a");
    if (!fp) {
        perror("fopen");
        return;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "Timestamp,Action,VID,PID,Vendor,Model\n");
    }

    fprintf(fp, "%s,%s,%s,%s,%s,%s\n",
            timestamp,
            action,
            vendor_id ? vendor_id : "Unknown",
            product_id ? product_id : "Unknown",
            vendor ? vendor : "Unknown",
            model ? model : "Unknown");

    fclose(fp);
}

int usb_monitor_init(void) {
    struct sockaddr_nl sa;
    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = getpid();
    sa.nl_groups = -1;

    if (bind(sock_fd, (struct sockaddr *)&sa, sizeof(sa)) < 0) {
        perror("bind");
        close(sock_fd);
        sock_fd = -1;
        return -1;
    }
    return 0;
}

int usb_monitor_wait_event(void) {
    if (sock_fd < 0) return -1;

    char buf[UEVENT_BUFFER_SIZE];
    int len = recv(sock_fd, buf, sizeof(buf), 0);
    if (len < 0) {
        perror("recv");
        return -1;
    }

    buf[len] = '\0';

    char *subsystem = get_value(buf, len, "SUBSYSTEM=");
    char *devtype   = get_value(buf, len, "DEVTYPE=");
    char *action    = get_value(buf, len, "ACTION=");

    if (subsystem && strcmp(subsystem, "usb") == 0 &&
        devtype && strcmp(devtype, "usb_device") == 0 &&
        action) {

        if (strcmp(action, "add") == 0) {
            log_usb_event("INSERTED", buf, len);
            currentEvent = EVENT_TYPE_PLUGIN; // Cập nhật sự kiện cắm usb
        } else if (strcmp(action, "remove") == 0) {
            log_usb_event("REMOVED", buf, len);
            currentEvent = EVENT_TYPE_PLUGOUT; // Cập nhật sự kiện rút usb
        }
    }

    return -1; // Không phải sự kiện USB device
}

void usb_monitor_close(void) {
    if (sock_fd >= 0) {
        close(sock_fd);
        sock_fd = -1;
    }
}
