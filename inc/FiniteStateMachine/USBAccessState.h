#ifndef USB_ACCESS_STATE_H
#define USB_ACCESS_STATE_H

struct USBAccessControlManager;
typedef struct USBAccessState USBAccessState;

struct USBAccessState {
    void (*onPlugin)(USBAccessState *state);
    void (*onPlugout)(USBAccessState *state);
    void (*onAllowAccessStorage)(USBAccessState *state);
    void (*onDenyAccess)(USBAccessState *state);
    void (*onCertNotVerify)(USBAccessState *state);
    void (*onCertVerified)(USBAccessState *state);
    void (*onMountSuccess)(USBAccessState *state);
    void (*onMountFailed)(USBAccessState *state);
    void (*onAllowAccessNonStorage)(USBAccessState *state);
    void (*setContext)(USBAccessState *state, struct USBAccessControlManager *manager);

    struct USBAccessControlManager *manager;
};

#endif