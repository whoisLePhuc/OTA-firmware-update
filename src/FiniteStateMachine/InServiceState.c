#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/InServiceState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugout(USBAccessState *state){
    printf("IN_SERVICE -> PLUGOUT. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onAllowAccessNonStorage(USBAccessState *state){
    printf("IN_SERVICE -> ALLOW_ACCESS_NON_STORAGE. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

// Function to create a new instance of the inServiceState
USBAccessState* inServiceStateCreate(){
    USBAccessState *inServiceState = (USBAccessState*)malloc(sizeof(USBAccessState));
    inServiceState->onPlugin = NULL;
    inServiceState->onPlugout = onPlugout;
    inServiceState->onAllowAccessStorage = NULL;
    inServiceState->onDenyAccess = NULL;
    inServiceState->onCertNotVerify = NULL;
    inServiceState->onCertVerified = NULL;
    inServiceState->onMountSuccess = NULL;
    inServiceState->onMountFailed = NULL;
    inServiceState->onAllowAccessNonStorage = onAllowAccessNonStorage;
    inServiceState->setContext = setContext;
    inServiceState->manager = NULL;
    return inServiceState;
}