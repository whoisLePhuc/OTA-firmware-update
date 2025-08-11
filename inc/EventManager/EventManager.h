#ifndef EVENT_MANAGE_H
#define EVENT_MANAGE_H

#include "FiniteStateMachine/USBAccessControlManager.h"

typedef enum {
    EVT_USB_NO_EVENT,
    EVT_USB_PLUGIN,
    EVT_USB_PLUGOUT,
    EVT_USB_ALLOW_ACCESS_STORAGE,
    EVT_USB_DENY_ACCESS,
    EVT_USB_CERT_NOT_VERIFY,
    EVT_USB_CERT_VERIFIED,
    EVT_USB_MOUNT_SUCCESS,
    EVT_USB_MOUNT_FAILED,
    EVT_USB_ALLOW_ACCESS_NON_STORAGE,
    EVT_USB_CERT_VERIFIED_SUCCESS,
    EVT_USB_CERT_VERIFIED_FAIL
} EventType;

extern EventType currentEvent;


void handleUsbEvent(EventType event, USBAccessControlManager *device);

#endif
