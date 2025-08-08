#include "USBAccessControlComponent/AccessControlListVerifier.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// So sánh không phân biệt hoa thường
static int str_casecmp(const char *a, const char *b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

// Xóa dòng cuối cùng trong usb_events.csv
static void remove_last_event(const char *events_file) {
    FILE *fp = fopen(events_file, "r");
    if (!fp) return;

    char lines[1024][512]; // giả sử tối đa 1024 dòng
    int count = 0;
    while (fgets(lines[count], sizeof(lines[count]), fp)) {
        count++;
    }
    fclose(fp);

    if (count <= 1) return; // chỉ có header hoặc file rỗng

    // Ghi lại trừ dòng cuối cùng
    fp = fopen(events_file, "w");
    if (!fp) return;

    for (int i = 0; i < count - 1; i++) {
        fputs(lines[i], fp);
    }
    fclose(fp);
}

// Hàm đọc dòng cuối cùng
static int read_last_event(const char *events_file,
                           char *vid, char *pid,
                           char *vendor, char *model) {
    FILE *fp = fopen(events_file, "r");
    if (!fp) return 0;

    char line[512];
    char last_line[512] = {0};

    while (fgets(line, sizeof(line), fp)) {
        if (strlen(line) > 1) {
            strcpy(last_line, line);
        }
    }
    fclose(fp);

    if (strlen(last_line) == 0) return 0;

    char timestamp[64], action[32];
    if (sscanf(last_line, "%63[^,],%31[^,],%63[^,],%63[^,],%127[^,],%127[^\n]",
               timestamp, action, vid, pid, vendor, model) == 6) {
        return 1;
    }
    return 0;
}

// Check whitelist và xoá record đã check
int check_whitelist(const char *events_file, const char *whitelist_file) {
    char vid[64], pid[64], vendor[128], model[128];
    if (!read_last_event(events_file, vid, pid, vendor, model)) {
        fprintf(stderr, "No event found in %s\n", events_file);
        return 0;
    }

    FILE *fp = fopen(whitelist_file, "r");
    if (!fp) {
        perror("fopen whitelist");
        return 0;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // bỏ header

    int result = 0; // mặc định: không hợp lệ
    while (fgets(line, sizeof(line), fp)) {
        char w_vid[64], w_pid[64], w_vendor[128], w_model[128];

        if (sscanf(line, "%63[^,],%63[^,],%127[^,],%127[^\n]",
                   w_vid, w_pid, w_vendor, w_model) == 4) {
            if ((str_casecmp(vid, w_vid) || strcmp(w_vid, "*") == 0) &&
                (str_casecmp(pid, w_pid) || strcmp(w_pid, "*") == 0) &&
                (str_casecmp(vendor, w_vendor) || strcmp(w_vendor, "*") == 0) &&
                (str_casecmp(model, w_model) || strcmp(w_model, "*") == 0)) {
                result = 1;
                break;
            }
        }
    }
    fclose(fp);

    // Xoá event sau khi check
    remove_last_event(events_file);

    return result;
}


