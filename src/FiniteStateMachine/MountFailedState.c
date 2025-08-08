#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/MountFailedState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/InServiceState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugout(USBAccessState *state){
    printf("MOUNT_FAILED: Nhan PLUGOUT. Chuyen sang UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onMountSuccess(USBAccessState *state){
    printf("MOUNT_FAILED: Nhan MOUNT_SUCCESS. Chuyen sang IN_SERVICE.\n");
    state->manager->setState(state->manager, inServiceStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

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