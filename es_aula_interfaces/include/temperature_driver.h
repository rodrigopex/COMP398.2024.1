
#include <stdbool.h>
#include <zephyr/kernel.h>

struct temperature_driver {
	uint8_t i;
	uint8_t q;
	bool is_celsius;
};

int temperature_driver_open(struct temperature_driver *self);

int temperature_driver_write(struct temperature_driver *self);

int temperature_driver_read(struct temperature_driver *self, double *temperature);

int temperature_driver_ioctl(struct temperature_driver *self, bool is_celsius);
