#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "EventManager/EventManager.h"

USBAccessControlManager *device = NULL;

void initUSBManager(void) {
    device = USBAccessControlManagerCreate(unpluggedStateCreate());
}

static void setState(USBAccessControlManager *manager, USBAccessState *newState){
    USBAccessState *oldState = manager->currentState;
    manager->currentState = newState;
    if(manager->currentState && manager->currentState->setContext) {
        manager->currentState->setContext(manager->currentState, manager);
    }
    if(oldState) {
        free(oldState);
    }
}

static void pluginEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onPlugin) {
        manager->currentState->onPlugin(manager->currentState);
    }
}

static void plugoutEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onPlugout) {
        manager->currentState->onPlugout(manager->currentState);
    }
}

static void allowAccessStorageEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onAllowAccessStorage) {
        manager->currentState->onAllowAccessStorage(manager->currentState);
    }
}

static void denyAccessEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onDenyAccess) {
        manager->currentState->onDenyAccess(manager->currentState);
    }
}

static void certNotVerifyEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onCertNotVerify) {
        manager->currentState->onCertNotVerify(manager->currentState);
    }
}

static void certVerifiedEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onCertVerified) {
        manager->currentState->onCertVerified(manager->currentState);
    }
}

static void mountSuccessEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onMountSuccess) {
        manager->currentState->onMountSuccess(manager->currentState);
    }
}

static void mountFailedEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onMountFailed) {
        manager->currentState->onMountFailed(manager->currentState);
    }
}

static void allowAccessNonStorageEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onAllowAccessNonStorage) {
        manager->currentState->onAllowAccessNonStorage(manager->currentState);
    }
}

USBAccessControlManager* USBAccessControlManagerCreate(USBAccessState *initialState){
    USBAccessControlManager *manager = (USBAccessControlManager*)malloc(sizeof(USBAccessControlManager));
    manager->currentState = initialState;
    manager->setState = setState;
    manager->pluginEvent = pluginEvent;
    manager->plugoutEvent = plugoutEvent;
    manager->allowAccessStorageEvent = allowAccessStorageEvent;
    manager->denyAccessEvent = denyAccessEvent;
    manager->certNotVerifyEvent = certNotVerifyEvent;
    manager->certVerifiedEvent = certVerifiedEvent;
    manager->mountSuccessEvent = mountSuccessEvent;
    manager->mountFailedEvent = mountFailedEvent;
    manager->allowAccessNonStorageEvent = allowAccessNonStorageEvent;
    if(initialState && initialState->setContext) {
        initialState->setContext(initialState, manager);
    }
    return manager;
}

void USBAccessControlManagerDestroy(USBAccessControlManager *manager){
    if(manager){
        if(manager->currentState){
            free(manager->currentState);
        }
        free(manager);
    }
}