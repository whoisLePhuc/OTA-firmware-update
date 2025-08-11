#include <stdio.h>
#include <unistd.h> 
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "EventManager/EventManager.h"
#include "USBAccessControlComponent/USBGuardInterface.h"
#include "USBAccessControlComponent/CertificateVerifier.h"

int main() {
    // Initialize USBAccessControlManager with initial state UnPlugged
    USBAccessControlManager *device = USBAccessControlManagerCreate(unpluggedStateCreate());
    if (!device) {
        fprintf(stderr, "Failed to create USBAccessControlManager\n");
        return 1;
    }
    DBusConnection* conn = usbguardInitConnection();

    if (!conn) return 1;
    printf("Listening USB events...\n");

    while (1) {
        if (usbguardListenEvent(conn, &usbInfo)) {
            usbguardPrintInfo(&usbInfo);
            if (usbInfo.event == 1) { 
                currentEvent = EVT_USB_PLUGIN;
            } else if (usbInfo.event == 3) {
                currentEvent = EVT_USB_PLUGOUT;
            }
        }
        handleUsbEvent(currentEvent, device);
        sleep(1); 
    }
    return 0;
}



