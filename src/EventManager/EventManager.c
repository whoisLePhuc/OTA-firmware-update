#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "EventManager/EventManager.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "USBAccessControlComponent/UdevListener.h"


EventType currentEvent = EVENT_TYPE_NONE;  // định nghĩa khởi tạo biến toàn cục

void handleEvent(EventType event){
    switch (event){
        case EVENT_TYPE_NONE:

            break;
        case EVENT_TYPE_PLUGIN:
            device->pluginEvent(device);
            break;
        case EVENT_TYPE_PLUGOUT:
            device->plugoutEvent(device);
            break;
        case EVENT_TYPE_ALLOW_ACCESS_STORAGE:
            device->allowAccessStorageEvent(device);
            break;
        case EVENT_TYPE_DENY_ACCESS:
            device->denyAccessEvent(device);
            break;
        case EVENT_TYPE_CERT_NOT_VERIFY:
            device->certNotVerifyEvent(device);
            break;
        case EVENT_TYPE_CERT_VERIFIED:
            device->certVerifiedEvent(device);
            break;
        case EVENT_TYPE_MOUNT_SUCCESS:
            device->mountSuccessEvent(device);
            break;
        case EVENT_TYPE_MOUNT_FAILED:
            device->mountFailedEvent(device);
            break;
        case EVENT_TYPE_ALLOW_ACCESS_NON_STORAGE:
            device->allowAccessNonStorageEvent(device);
            break;
        default:
            printf("EventManager: Sự kiện không xác định.\n");
            break;
}
}

