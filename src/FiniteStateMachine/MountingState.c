#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/MountingState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/InServiceState.h"
#include "FiniteStateMachine/MountFailedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"

static void onPlugout(USBAccessState *state){
    printf("MOUNTING: Nhan PLUGOUT. Chuyen sang UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onMountSuccess(USBAccessState *state){
    printf("MOUNTING: Nhan MOUNT_SUCCESS. Chuyen sang IN_SERVICE.\n");
    state->manager->setState(state->manager, inServiceStateCreate());
}

static void onMountFailed(USBAccessState *state){
    printf("MOUNTING: Nhan MOUNT_FAILED. Chuyen sang MOUNT_FAILED.\n");
    state->manager->setState(state->manager, mountFailedStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

USBAccessState* mountingStateCreate(){
    USBAccessState *mountingState = (USBAccessState*)malloc(sizeof(USBAccessState));
    mountingState->onPlugin = NULL;
    mountingState->onPlugout = onPlugout;
    mountingState->onAllowAccessStorage = NULL;
    mountingState->onDenyAccess = NULL;
    mountingState->onCertNotVerify = NULL;
    mountingState->onCertVerified = NULL;
    mountingState->onMountSuccess = onMountSuccess;
    mountingState->onMountFailed = onMountFailed;
    mountingState->onAllowAccessNonStorage = NULL;
    mountingState->setContext = setContext;
    mountingState->manager = NULL;
    return mountingState;
}