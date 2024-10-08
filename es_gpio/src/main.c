/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/device.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/printk.h>
#include <inttypes.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

/*
 * Get button configuration from the devicetree sw0 alias. This is mandatory.
 */
#define SW0_NODE DT_ALIAS(sw0)
#if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
#error "Unsupported board: sw0 devicetree alias is not defined"
#endif
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});
static struct gpio_callback button_cb_data;

K_SEM_DEFINE(toggle_sem, 1, 1);

/*
 * The led0 devicetree alias is optional. If present, we'll use it
 * to turn on the LED whenever the button is pressed.
 */

void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
	printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
	k_sem_give(&toggle_sem);
}

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	if (!gpio_is_ready_dt(&button)) {
		printk("Error: button device %s is not ready\n", button.port->name);
		return 0;
	}

	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure %s pin %d\n", ret, button.port->name,
		       button.pin);
		return 0;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret != 0) {
		printk("Error %d: failed to configure interrupt on %s pin %d\n", ret,
		       button.port->name, button.pin);
		return 0;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);
	printk("Set up button at %s pin %d\n", button.port->name, button.pin);

	while (1) {
		k_sem_take(&toggle_sem, K_FOREVER);
		led_state = !led_state;
		gpio_pin_set_dt(&led, led_state);
		if (ret < 0) {
			return 0;
		}
		printf("LED state: %s\n", led_state ? "ON" : "OFF");
	}
	return 0;
}

// int main(void)
// {
//
//
//
//
// 	if (led.port && !gpio_is_ready_dt(&led)) {
// 		printk("Error %d: LED device %s is not ready; ignoring it\n", ret, led.port->name);
// 		led.port = NULL;
// 	}
// 	if (led.port) {
// 		ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
// 		if (ret != 0) {
// 			printk("Error %d: failed to configure LED device %s pin %d\n", ret,
// 			       led.port->name, led.pin);
// 			led.port = NULL;
// 		} else {
// 			printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
// 		}
// 	}
//
// 	printk("Press the button\n");
// 	if (led.port) {
// 		while (1) {
// 			/* If we have an LED, match its state to the button's. */
// 			int val = gpio_pin_get_dt(&button);
//
// 			if (val >= 0) {
// 				gpio_pin_set_dt(&led, val);
// 			}
// 			k_msleep(SLEEP_TIME_MS);
// 		}
// 	}
// 	return 0;
// }
