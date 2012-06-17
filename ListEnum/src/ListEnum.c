#include <libusb-1.0/libusb.h>
#include <stdio.h>

int main(int ac, char **av) {
	libusb_context *context;
	libusb_device **devices;
	struct libusb_device_descriptor device_desc;
	int status;
	int i;
	unsigned char manufacturer[128];
	unsigned char product[128];
	int length;
	libusb_device_handle *handle;

	status = libusb_init(&context);
	if (status) {
		printf ("libusb_init: failed with code=%s\n", libusb_error_name(status));
		return 0;
	}
	int nDevices = libusb_get_device_list(context, &devices);
	if (status < 0) {
		printf ("libusb_get_device_list: failed with code=%d\n", status);
	} else {
		for (i = 0; i < nDevices; i++) {
			libusb_device *device = devices[i];
			status = libusb_get_device_descriptor(device, &device_desc);
			if (status) {
				printf ("libusb_get_device_descriptor: failed with code=%s\n", libusb_error_name(status));
			} else {
				printf ("VID=%04X/PID=%04X  ", device_desc.idVendor, device_desc.idProduct);
				status = libusb_open(device, &handle);
				if (status < 0) {
					printf ("libusb_open: failed with code=%s\n", libusb_error_name(status));
				} else {
					length = libusb_get_string_descriptor_ascii(handle, device_desc.iManufacturer, manufacturer, sizeof manufacturer - 2);
					if (length < 0) {
						printf ("libusb_get_string_descriptor_ascii: failed with code=%s\n", libusb_error_name(length));
					} else {
						manufacturer[length] = 0;
						printf ("%s  ", manufacturer);
					}
					length = libusb_get_string_descriptor_ascii(handle, device_desc.iProduct, product, sizeof product - 2);
					if (length < 0) {
						printf ("libusb_get_string_descriptor_ascii: failed with code=%s\n", libusb_error_name(length));
					} else {
						product[length] = 0;
						printf ("%s  ", product);
					}
					libusb_close(handle);
				}
				printf ("\n");
			}
		}
		libusb_free_device_list(devices, 1);
	}
	libusb_exit(context);
	return (0);
}
