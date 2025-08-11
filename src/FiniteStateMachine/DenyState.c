#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "EventManager/EventManager.h"


static void onPlugout(USBAccessState *state){
    printf("DENY -> PLUGOUT. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;

    // Các hàm được phép thực hiện trong trạng thái DENY
}

// Function to create a new instance of the DenyState
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