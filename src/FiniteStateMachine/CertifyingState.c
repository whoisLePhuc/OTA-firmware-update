#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/CertifyingState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/MountingState.h"
#include "EventManager/EventManager.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "USBAccessControlComponent/UdevListener.h"

static void onPlugin(USBAccessState *state) {
    printf("CERTIFYING: Nhan PLUGIN. Khong co thay doi.\n");
}

static void onPlugout(USBAccessState *state){
    printf("CERTIFYING: Nhan PLUGOUT. Chuyen sang UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onCertNotVerify(USBAccessState *state){
    printf("CERTIFYING: Nhan CERTIFICATE_NOT_VERIFY. Chuyen sang DENY.\n");
    state->manager->setState(state->manager, denyStateCreate());
}

static void onCertVerified(USBAccessState *state){
    printf("CERTIFYING: Nhan CERTIFICATE_VERIFIED. Chuyen sang MOUNTING.\n");
    state->manager->setState(state->manager, mountingStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;

    // Các hàm được thực hiện trong trạng thái CERTIFYING
    testCertifyingState();
}

void testCertifyingState(){
    currentEvent = EVENT_TYPE_CERT_NOT_VERIFY; // Giả lập sự kiện CERT_VERIFIED
}

USBAccessState* certifyingStateCreate(){
    USBAccessState *certifyingState = (USBAccessState*)malloc(sizeof(USBAccessState));
    certifyingState->onPlugin = onPlugin;
    certifyingState->onPlugout = onPlugout;
    certifyingState->onAllowAccessStorage = NULL;
    certifyingState->onDenyAccess = NULL;
    certifyingState->onCertNotVerify = onCertNotVerify;
    certifyingState->onCertVerified = onCertVerified;
    certifyingState->onMountSuccess = NULL;
    certifyingState->onMountFailed = NULL;
    certifyingState->onAllowAccessNonStorage = NULL;
    certifyingState->setContext = setContext;
    certifyingState->manager = NULL;
    return certifyingState;
}