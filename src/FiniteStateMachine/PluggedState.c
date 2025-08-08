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
    printf("PLUGGED: Nhan PLUGIN. Khong co thay doi.\n");
}

static void onPlugout(USBAccessState *state){
    printf("PLUGGED: Nhan PLUGOUT. Chuyen sang UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onAllowAccessStorage(USBAccessState *state){
    printf("PLUGGED: Nhan ALLOW_ACCESS_STORAGE. Chuyen sang CERTIFYING.\n");
    state->manager->setState(state->manager, certifyingStateCreate());
}

static void onDenyAccess(USBAccessState *state){
    printf("PLUGGED: Nhan DENY_ACCESS. Chuyen sang DENY.\n");
    state->manager->setState(state->manager, denyStateCreate());
}

static void onAllowAccessNonStorage(USBAccessState *state){
    printf("PLUGGED: Nhan ALLOW_ACCESS_NON_STORAGE. Chuyen sang ALLOW_ACCESS_NON_STORAGE.\n");
    state->manager->setState(state->manager, mountingStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;

    // Các function được thực hiện khi ở trạng thái PLUGGED
    // Kiểm tra thiết bị có phải là thiết bị lưu trữ hay không
    if (usbguardStorageDeviceCheck(&usbInfo)) {
        currentEvent = EVENT_TYPE_ALLOW_ACCESS_STORAGE;
    } else {
        currentEvent = EVENT_TYPE_ALLOW_ACCESS_NON_STORAGE;
    }
    
}

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