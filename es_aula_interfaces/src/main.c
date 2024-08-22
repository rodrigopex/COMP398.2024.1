/*
 * Copyright (c) 2024 IC UFAL
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>

#include "led_driver.h"
#include "temperature_driver.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, 4);

int main(void)
{
	// temperature_driver_open(NULL);

	struct temperature_driver temp1;
	temperature_driver_open(&temp1);
	temp1.i = 33;
	temp1.q = 57;

	struct temperature_driver temp2;
	temperature_driver_open(&temp2);
	temp2.i = 23;
	temp2.q = 19;

	led_driver_open();
	led_driver_ioctl(10);

	while (1) {

		led_driver_write(BLINK);
		double t1;
		temperature_driver_read(&temp1, &t1);

		double t2;
		temperature_driver_read(&temp2, &t2);

		LOG_DBG("Temperature t1 = %.02f, t2 = %.02f", t1, t2);

		k_msleep(1000);
	}
	return 0;
}
