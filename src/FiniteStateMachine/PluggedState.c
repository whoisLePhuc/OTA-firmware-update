#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/PluggedState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/CertifyingState.h"
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "USBAccessControlComponent/AccessControlListVerifier.h"
#include "EventManager/EventManager.h"

#define EVENTS_FILE "/home/lephuc/OTA-firmware-update/csv_file/ConnectDevice.csv"
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

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;

    if (check_whitelist(EVENTS_FILE, WHITELIST_FILE)) {
        printf(">> Device is in WHITELIST ✅\n");
        currentEvent = EVENT_TYPE_ALLOW_ACCESS_STORAGE; // Cập nhật sự kiện cho phép truy cập lưu trữ
    } else {
        printf(">> Device is NOT in whitelist ❌\n");
        currentEvent = EVENT_TYPE_DENY_ACCESS; // Cập nhật sự kiện từ chối truy cập
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
    pluggedState->onAllowAccessNonStorage = NULL;
    pluggedState->setContext = setContext;
    pluggedState->manager = NULL;
    return pluggedState;
}