#ifndef USB_MONITOR_H
#define USB_MONITOR_H

// Khởi tạo socket lắng nghe sự kiện udev
int usb_monitor_init(void);

// Hàm chặn (blocking) để chờ sự kiện USB.
// Trả về 1 nếu INSERTED, 0 nếu REMOVED, -1 nếu lỗi.
int usb_monitor_wait_event(void);

// Đóng socket khi không dùng nữa
void usb_monitor_close(void);

#endif
