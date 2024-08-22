#include "temperature_driver.h"

#include <zephyr/sys/check.h>

int temperature_driver_open(struct temperature_driver *self)
{
	CHECKIF(self == NULL) {
		return -EINVAL;
	}

	self->i = 0;
	self->q = 0;

	return 0;
}

int temperature_driver_write(struct temperature_driver *self)
{
	__ASSERT_UNREACHABLE;
	return -ENOTSUP;
}

int temperature_driver_read(struct temperature_driver *self, double *temperature)
{
	CHECKIF(self == NULL) {
		return -EINVAL;
	}

	*temperature = self->i + self->q / 100.0;

	return 0;
}

int temperature_driver_ioctl(struct temperature_driver *self, bool is_celsius)
{
	CHECKIF(self == NULL) {
		return -EINVAL;
	}

	self->is_celsius = is_celsius;

	return 0;
}
