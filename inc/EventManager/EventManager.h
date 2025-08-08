#ifndef EVENT_MANAGE_H
#define EVENT_MANAGE_H

#include "FiniteStateMachine/USBAccessControlManager.h"

typedef enum {
    EVENT_TYPE_NONE,
    EVENT_TYPE_PLUGIN,
    EVENT_TYPE_PLUGOUT,
    EVENT_TYPE_ALLOW_ACCESS_STORAGE,
    EVENT_TYPE_DENY_ACCESS,
    EVENT_TYPE_CERT_NOT_VERIFY,
    EVENT_TYPE_CERT_VERIFIED,
    EVENT_TYPE_MOUNT_SUCCESS,
    EVENT_TYPE_MOUNT_FAILED,
    EVENT_TYPE_ALLOW_ACCESS_NON_STORAGE,
    EVENT_TYPE_CERT_VERIFIED_SUCCESS,
    EVENT_TYPE_CERT_VERIFIED_FAIL
} EventType;

extern EventType currentEvent;   // khai báo biến toàn cục

void handleEvent(EventType event, USBAccessControlManager *device);

#endif
