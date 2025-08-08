#include <stdio.h>
#include <unistd.h> 
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "USBAccessControlComponent/UdevListener.h"
#include "EventManager/EventManager.h"
#include "USBAccessControlComponent/USBGuardInterface.h"

int main() {
    // Khởi tạo USBAccessControlManager với trạng thái ban đầu là UnPlugged
    USBAccessControlManager *device = USBAccessControlManagerCreate(unpluggedStateCreate());
    if (!device) {
        fprintf(stderr, "Failed to create USBAccessControlManager\n");
        return 1;
    }
    DBusConnection* conn = usbguardInitConnection();

    if (!conn) return 1;
    printf("Đang lắng nghe USB events...\n");

    while (1) {
        if (usbguardListenEvent(conn, &usbInfo)) {
            usbguardPrintInfo(&usbInfo);

            if (usbInfo.event == 1) { 
                currentEvent = EVENT_TYPE_PLUGIN;
            } else if (usbInfo.event == 3) {
                currentEvent = EVENT_TYPE_PLUGOUT;
            }
  
            // Xử lý sự kiện USB
            
            // Ở đây có thể gọi các module khác xử lý usbInfo
        }
        handleEvent(currentEvent, device);
        sleep(1); 
    }
    return 0;
}

