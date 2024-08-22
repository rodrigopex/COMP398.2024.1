/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <stdio.h>
#include <zephyr/drivers/gpio.h>

#include "led_driver.h"

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(led_driver, 4);

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

static struct {
	bool is_on;
	const struct gpio_dt_spec led;
	int blink_duration;
} self = {
	.is_on = false,
	.led = GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	.blink_duration = 1000,
};

int led_driver_open()
{
	if (!gpio_is_ready_dt(&self.led)) {
		return -ENODEV;
	}

	int ret = gpio_pin_configure_dt(&self.led, GPIO_OUTPUT_ACTIVE);

	if (ret < 0) {
		return ret;
	}

	return 0;
}

static inline void update_led_state()
{
	gpio_pin_set_dt(&self.led, self.is_on);
	LOG_DBG("LED state: %s", self.is_on ? "ON" : "OFF");
}

int led_driver_write(enum led_driver_action action)
{
	int ret = 0;
	switch (action) {
	case OFF:
		self.is_on = false;
		update_led_state();
		break;
	case ON:
		self.is_on = true;
		update_led_state();
		break;
	case TOGGLE:
		self.is_on = !self.is_on;
		update_led_state();
		break;
	case BLINK:
		self.is_on = !self.is_on;
		update_led_state();
		k_msleep(self.blink_duration);
		self.is_on = !self.is_on;
		update_led_state();
		break;
	}

	return ret;
}

int led_driver_read(bool *is_on)
{
	if (is_on == NULL) {
		return -EINVAL;
	}

	*is_on = self.is_on;

	return 0;
}

int led_driver_ioctl(int duration)
{
	self.blink_duration = duration;

	return 0;
}
