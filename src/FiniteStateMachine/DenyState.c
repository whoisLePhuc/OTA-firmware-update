#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"


static void onPlugout(USBAccessState *state){
    printf("DENY: Nhan PLUGOUT. Chuyen sang UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
}

USBAccessState* denyStateCreate(){
    USBAccessState *denyState = (USBAccessState*)malloc(sizeof(USBAccessState));
    denyState->onPlugin = NULL;
    denyState->onPlugout = onPlugout;
    denyState->onAllowAccessStorage = NULL;
    denyState->onDenyAccess = NULL;
    denyState->onCertNotVerify = NULL;
    denyState->onCertVerified = NULL;
    denyState->onMountSuccess = NULL;
    denyState->onMountFailed = NULL;
    denyState->onAllowAccessNonStorage = NULL;
    denyState->setContext = setContext;
    denyState->manager = NULL;
    return denyState;
}