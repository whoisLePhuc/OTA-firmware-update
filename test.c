#include <stdio.h>
#include <unistd.h> 
#include "inc/FiniteStateMachine/USBAccessControlManager.h"
#include "inc/FiniteStateMachine/UnPluggedState.h"



int main(){
    printf("Khoi tao FiniteStateMachine: Trang thai ban dau UNPLUGGED.\n");
    USBAccessControlManager *device = USBAccessControlManagerCreate(unpluggedStateCreate());

    printf("\n--- Test case 1: Happy path ---\n");
    device->pluginEvent(device);
    sleep(1);
    device->allowAccessStorageEvent(device);
    sleep(1);
    device->certVerifiedEvent(device);
    sleep(1);
    device->mountSuccessEvent(device);
    sleep(1);
    device->plugoutEvent(device);
    sleep(1);

    printf("\n--- Test case 2: Certificate failed ---\n");
    device->pluginEvent(device);
    sleep(1);
    device->allowAccessStorageEvent(device);
    sleep(1);
    device->certNotVerifyEvent(device);
    sleep(1);
    device->plugoutEvent(device);
    sleep(1);

    printf("\n--- Test case 3: Mounting failed ---\n");
    device->pluginEvent(device);
    sleep(1);
    device->allowAccessStorageEvent(device);
    sleep(1);
    device->certVerifiedEvent(device);
    sleep(1);
    device->mountFailedEvent(device);
    sleep(1);
    device->plugoutEvent(device);
    sleep(1);

    printf("\n--- Test case 4: Plugout during mounting ---\n");
    device->pluginEvent(device);
    sleep(1);
    device->allowAccessStorageEvent(device);
    sleep(1);
    device->certVerifiedEvent(device);
    sleep(1);
    device->plugoutEvent(device);
    sleep(1);

    USBAccessControlManagerDestroy(device);
    printf("\nChuong trinh ket thuc.\n");

    return 0;
}