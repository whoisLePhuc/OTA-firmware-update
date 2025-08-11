#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "EventManager/EventManager.h"

// Function prototypes for state transition functions
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

// pluginvent handling functions
static void pluginEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onPlugin) {
        manager->currentState->onPlugin(manager->currentState);
    }
}

// plugout handling functions
static void plugoutEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onPlugout) {
        manager->currentState->onPlugout(manager->currentState);
    }
}


// allowAccessStorageEvent handling functions
static void allowAccessStorageEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onAllowAccessStorage) {
        manager->currentState->onAllowAccessStorage(manager->currentState);
    }
}

// denyAccessEvent handling functions
static void denyAccessEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onDenyAccess) {
        manager->currentState->onDenyAccess(manager->currentState);
    }
}

// certNotVerifyEvent handling functions
static void certNotVerifyEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onCertNotVerify) {
        manager->currentState->onCertNotVerify(manager->currentState);
    }
}

// certVerifiedEvent handling functions
static void certVerifiedEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onCertVerified) {
        manager->currentState->onCertVerified(manager->currentState);
    }
}

// mountSuccessEvent handling functions
static void mountSuccessEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onMountSuccess) {
        manager->currentState->onMountSuccess(manager->currentState);
    }
}


// mountFailedEvent handling functions
static void mountFailedEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onMountFailed) {
        manager->currentState->onMountFailed(manager->currentState);
    }
}

// allowAccessNonStorageEvent handling functions
static void allowAccessNonStorageEvent(USBAccessControlManager *manager){
    if(manager->currentState && manager->currentState->onAllowAccessNonStorage) {
        manager->currentState->onAllowAccessNonStorage(manager->currentState);
    }
}

// Function to create a new USBAccessControlManager instance
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

// Function to destroy the USBAccessControlManager instance
void USBAccessControlManagerDestroy(USBAccessControlManager *manager){
    if(manager){
        if(manager->currentState){
            free(manager->currentState);
        }
        free(manager);
    }
}