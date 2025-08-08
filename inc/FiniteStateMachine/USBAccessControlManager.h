#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "USBAccessState.h"

typedef struct USBAccessControlManager {
    USBAccessState *currentState;
    void (*setState)(struct USBAccessControlManager *manager, USBAccessState *newState);
    void (*pluginEvent)(struct USBAccessControlManager *manager);
    void (*plugoutEvent)(struct USBAccessControlManager *manager);
    void (*allowAccessStorageEvent)(struct USBAccessControlManager *manager);
    void (*denyAccessEvent)(struct USBAccessControlManager *manager);
    void (*certNotVerifyEvent)(struct USBAccessControlManager *manager);
    void (*certVerifiedEvent)(struct USBAccessControlManager *manager);
    void (*mountSuccessEvent)(struct USBAccessControlManager *manager);
    void (*mountFailedEvent)(struct USBAccessControlManager *manager);
    void (*allowAccessNonStorageEvent)(struct USBAccessControlManager *manager);
} USBAccessControlManager;

USBAccessControlManager* USBAccessControlManagerCreate(USBAccessState *initialState);

void USBAccessControlManagerDestroy(USBAccessControlManager *manager);
void initUSBManager(void);

// Biến toàn cục
extern USBAccessControlManager *device;

#endif // DEVICE_MANAGER_H