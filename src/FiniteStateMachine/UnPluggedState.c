#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/PluggedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugin(USBAccessState *state){
    printf("UNPLUGGED: Nhan PLUGIN. Chuyen sang PLUGGED.\n");
    state->manager->setState(state->manager, pluggedStateCreate());
}

static void onPlugout(USBAccessState *state){
    printf("UNPLUGGED: Nhan PLUGOUT. Khong co thay doi.\n");
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

USBAccessState* unpluggedStateCreate(){
    USBAccessState *unpluggedState = (USBAccessState*)malloc(sizeof(USBAccessState));
    unpluggedState->onPlugin = onPlugin;
    unpluggedState->onPlugout = onPlugout;
    unpluggedState->onAllowAccessStorage = NULL;
    unpluggedState->onDenyAccess = NULL;
    unpluggedState->onCertNotVerify = NULL;
    unpluggedState->onCertVerified = NULL;
    unpluggedState->onMountSuccess = NULL;
    unpluggedState->onMountFailed = NULL;
    unpluggedState->onAllowAccessNonStorage = NULL;
    unpluggedState->setContext = setContext;
    unpluggedState->manager = NULL;
    return unpluggedState;
}