#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <libusb.h>
 
static int count = 0;
 
int hotplug_callback(struct libusb_context *ctx, struct libusb_device *dev,
                     libusb_hotplug_event event, void *user_data) {
  static libusb_device_handle *dev_handle = NULL;
  struct libusb_device_descriptor desc;
  int rc;
 
  printf("CallBack Called! \n");

  (void)libusb_get_device_descriptor(dev, &desc);
 
  if (LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED == event) {

    printf("Hellow! \n");
    rc = libusb_open(dev, &dev_handle);
    if (LIBUSB_SUCCESS != rc) {
      printf("Could not open USB device\n");
    }
  } else if (LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT == event) {
    printf("bye! \n");
    if (dev_handle) {
      //libusb_close(dev_handle);
      //dev_handle = NULL;
    }
  } else {
    printf("Unhandled event %d\n", event);
  }
  count++;
 
  return 0;
}
 
int main (void) {
  libusb_hotplug_callback_handle callback_handle;
  int rc;
  struct libusb_context *ctx = NULL;  
    const struct libusb_init_option options[] = {
        {
            .option = LIBUSB_OPTION_LOG_LEVEL,
            .value = {.ival = LIBUSB_LOG_LEVEL_DEBUG} 
        }
    };
  libusb_init_context(&ctx, options, 1);

  // Verify platform supported 
  if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
      printf("Hotplug capabilities are not supported on this platform.\n");
      libusb_exit(NULL);
      return 1;
  }
 
  rc = libusb_hotplug_register_callback( ctx, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED |
                                        LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_ENUMERATE, 
                                        0x67b,
                                        0x2303,
                                        LIBUSB_HOTPLUG_MATCH_ANY, hotplug_callback, NULL,
                                        &callback_handle);
  if (LIBUSB_SUCCESS != rc) {
    printf("Error creating a hotplug callback\n");
    libusb_exit(NULL);
    return EXIT_FAILURE;
  }
 
  while (true) {
    printf("%d..\n", count);
    libusb_handle_events_timeout_completed(ctx, &(struct timeval){30, 0}, NULL);
    //libusb_handle_events_completed(NULL, NULL);
  }
 
  libusb_hotplug_deregister_callback(NULL, callback_handle);
  libusb_exit(NULL);
 
  return 0;
}
