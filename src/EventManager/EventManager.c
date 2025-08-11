#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "EventManager/EventManager.h"
#include "FiniteStateMachine/USBAccessControlManager.h"

EventType currentEvent = EVT_USB_NO_EVENT;  // Initialize global current event variable

void handleUsbEvent(EventType event, USBAccessControlManager *device) {
    switch (event){
        case EVT_USB_NO_EVENT:
            break;
        case EVT_USB_PLUGIN:
            device->pluginEvent(device);
            break;
        case EVT_USB_PLUGOUT:
            device->plugoutEvent(device);
            break;
        case EVT_USB_ALLOW_ACCESS_STORAGE:
            device->allowAccessStorageEvent(device);
            break;
        case EVT_USB_DENY_ACCESS:
            device->denyAccessEvent(device);
            break;
        case EVT_USB_CERT_NOT_VERIFY:
            device->certNotVerifyEvent(device);
            break;
        case EVT_USB_CERT_VERIFIED:
            device->certVerifiedEvent(device);
            break;
        case EVT_USB_MOUNT_SUCCESS:
            device->mountSuccessEvent(device);
            break;
        case EVT_USB_MOUNT_FAILED:
            device->mountFailedEvent(device);
            break;
        case EVT_USB_ALLOW_ACCESS_NON_STORAGE:
            device->allowAccessNonStorageEvent(device);
            break;
        default:
            printf("EventManager: unavailable event.\n");
            break;
}
}

