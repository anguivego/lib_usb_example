#include <stdio.h>
#include "libusb.h"

#define VID 0x067b // Replace with your device's Vendor ID
#define PID 0x2303 // Replace with your device's Product ID
#define INTERFACE_NUM 0 

int main() {
    libusb_context *ctx = NULL;
    libusb_device_handle *handle = NULL;

    if (libusb_init(&ctx) < 0) return 1;

    handle = libusb_open_device_with_vid_pid(ctx, VID, PID);
    if (!handle) {
        libusb_exit(ctx);
        return 1;
    }

    // Detach the native kernel serial driver if active
    if (libusb_kernel_driver_active(handle, INTERFACE_NUM) == 1) {
        libusb_detach_kernel_driver(handle, INTERFACE_NUM);
    }

    // Claim the interface to lock communication
    libusb_claim_interface(handle, INTERFACE_NUM);

    // ... Driver logic goes here ...

    libusb_release_interface(handle, INTERFACE_NUM);
    libusb_close(handle);
    libusb_exit(ctx);
    return 0;
}
