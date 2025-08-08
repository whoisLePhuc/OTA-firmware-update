#include <stdio.h>
#include <unistd.h> 
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "USBAccessControlComponent/UdevListener.h"
#include "EventManager/EventManager.h"
#include "USBAccessControlComponent/USBGuardInterface.h"

void usb_event_handler(UsbDeviceEvent* device) {
    printf("Thiết bị USB:\n");
    printf("  ID: %u\n", device->id);
    printf("  Event: %u\n", device->event);
    printf("  Target: %u\n", device->target);
    printf("  Vendor ID: %s\n", device->vendor_id);
    printf("  Product ID: %s\n", device->product_id);
    printf("  Rule: %s\n\n", device->rule);
}

int main() {
    listen_usbguard_events(usb_event_handler);
    return 0;
}


/*
int main(){
    if (usb_monitor_init() < 0) {
        printf("Failed to init USB monitor\n");
        return -1;
    }

    printf("Khoi tao FSM: Trang thai ban dau UNPLUGGED.\n");
    initUSBManager();

    while (1) {
        usb_monitor_wait_event();
        handleEvent(currentEvent);
        sleep(0.01);
    }

    USBAccessControlManagerDestroy(device);
    printf("\nChuong trinh ket thuc.\n");

    return 0;
}
*/
