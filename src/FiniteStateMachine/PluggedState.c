#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/PluggedState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/CertifyingState.h"
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/MountingState.h"
#include "USBAccessControlComponent/AccessControlListVerifier.h"
#include "USBAccessControlComponent/USBGuardInterface.h"
#include "EventManager/EventManager.h"


#define WHITELIST_FILE "/home/lephuc/OTA-firmware-update/csv_file/whitelist.csv"

static void onPlugin(USBAccessState *state) {
    printf("PLUGGED -> PLUGIN. No change.\n");
}

static void onPlugout(USBAccessState *state){
    printf("PLUGGED -> PLUGOUT. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onAllowAccessStorage(USBAccessState *state){
    printf("PLUGGED -> ALLOW_ACCESS_STORAGE. Change to CERTIFYING.\n");
    state->manager->setState(state->manager, certifyingStateCreate());
}

static void onDenyAccess(USBAccessState *state){
    printf("PLUGGED -> DENY_ACCESS. Change to DENY.\n");
    state->manager->setState(state->manager, denyStateCreate());
}

static void onAllowAccessNonStorage(USBAccessState *state){
    printf("PLUGGED -> ALLOW_ACCESS_NON_STORAGE. Change to ALLOW_ACCESS_NON_STORAGE.\n");
    state->manager->setState(state->manager, mountingStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;

    // Check if the USB device is a storage device
    if (usbguardStorageDeviceCheck(&usbInfo)) {
        currentEvent = EVT_USB_ALLOW_ACCESS_STORAGE;
    } else {
        currentEvent = EVT_USB_ALLOW_ACCESS_NON_STORAGE;
    }
}

// Function to create a new instance of the PluggedState
USBAccessState* pluggedStateCreate(){
    USBAccessState *pluggedState = (USBAccessState*)malloc(sizeof(USBAccessState));
    pluggedState->onPlugin = onPlugin;
    pluggedState->onPlugout = onPlugout;
    pluggedState->onAllowAccessStorage = onAllowAccessStorage;
    pluggedState->onDenyAccess = onDenyAccess;
    pluggedState->onCertNotVerify = NULL;
    pluggedState->onCertVerified = NULL;
    pluggedState->onMountSuccess = NULL;
    pluggedState->onMountFailed = NULL;
    pluggedState->onAllowAccessNonStorage = onAllowAccessNonStorage;
    pluggedState->setContext = setContext;
    pluggedState->manager = NULL;
    return pluggedState;
}