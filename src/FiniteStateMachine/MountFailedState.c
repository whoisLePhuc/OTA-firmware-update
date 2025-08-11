#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/MountFailedState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/InServiceState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugout(USBAccessState *state){
    printf("MOUNT_FAILED -> PLUGOUT. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onMountSuccess(USBAccessState *state){
    printf("MOUNT_FAILED -> MOUNT_SUCCESS. Change to IN_SERVICE.\n");
    state->manager->setState(state->manager, inServiceStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

// Function to create a new instance of the MountFailedState
USBAccessState* mountFailedStateCreate(){
    USBAccessState *mountFailedState = (USBAccessState*)malloc(sizeof(USBAccessState));
    mountFailedState->onPlugin = NULL;
    mountFailedState->onPlugout = onPlugout;
    mountFailedState->onAllowAccessStorage = NULL;
    mountFailedState->onDenyAccess = NULL;
    mountFailedState->onCertNotVerify = NULL;
    mountFailedState->onCertVerified = NULL;
    mountFailedState->onMountSuccess = onMountSuccess;
    mountFailedState->onMountFailed = NULL;
    mountFailedState->onAllowAccessNonStorage = NULL;
    mountFailedState->setContext = setContext;
    mountFailedState->manager = NULL;
    return mountFailedState;
}