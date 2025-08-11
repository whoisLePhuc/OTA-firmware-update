#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/PluggedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugin(USBAccessState *state){
    printf("UNPLUGGED -> PLUGIN. Change to PLUGGED.\n");
    state->manager->setState(state->manager, pluggedStateCreate());
}

static void onPlugout(USBAccessState *state){
    printf("UNPLUGGED -> Nhan PLUGOUT.No change.\n");
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

// Function to create a new instance of the UnPluggedState
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