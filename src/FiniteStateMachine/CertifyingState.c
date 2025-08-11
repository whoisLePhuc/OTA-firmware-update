#include <stdio.h>
#include <stdlib.h>
#include "FiniteStateMachine/CertifyingState.h"
#include "FiniteStateMachine/UnPluggedState.h"
#include "FiniteStateMachine/DenyState.h"
#include "FiniteStateMachine/MountingState.h"
#include "EventManager/EventManager.h"
#include "FiniteStateMachine/USBAccessControlManager.h"
#include "USBAccessControlComponent/CertificateVerifier.h"

static void onPlugin(USBAccessState *state) {
    printf("CERTIFYING -> PLUGIN. No change.\n");
}

static void onPlugout(USBAccessState *state){
    printf("CERTIFYING -> PLUGOUT. Change to UNPLUGGED.\n");
    state->manager->setState(state->manager, unpluggedStateCreate());
}

static void onCertNotVerify(USBAccessState *state){
    printf("CERTIFYING -> CERTIFICATE_NOT_VERIFY. Change to DENY.\n");
    state->manager->setState(state->manager, denyStateCreate());
}

static void onCertVerified(USBAccessState *state){
    printf("CERTIFYING -> CERTIFICATE_VERIFIED. Change to MOUNTING.\n");
    state->manager->setState(state->manager, mountingStateCreate());
}

static void setContext(USBAccessState *state, struct USBAccessControlManager *manager){
    state->manager = manager;
    certifyingUsbCert();
}

// Function to verify the USB certificate
void certifyingUsbCert(){
    if (certgen_verify_cert_with_ca("input/usb_cert.pem", "cert/ca.crt") == 1) {
        currentEvent = EVT_USB_CERT_VERIFIED;
        printf("Certificate is valid and signed by CA.\n");
    } else {
        currentEvent = EVT_USB_CERT_NOT_VERIFY;
        printf("Verification failed: %s\n");
    }
}

// Function to create a new instance of the CertifyingState
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