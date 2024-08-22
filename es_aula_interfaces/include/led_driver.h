#include <stdbool.h>
#include <zephyr/kernel.h>

enum led_driver_action {
	OFF,
	ON,
	TOGGLE,
	BLINK,
};

int led_driver_open();

int led_driver_write(enum led_driver_action action);

/**
 * @brief function to retrieve the led state
 *
 * @param[out] is_on
 * @retval 0 success
 * @retval -EINVAL invalid reference
 */
int led_driver_read(bool *is_on);

int led_driver_ioctl(int duration);
