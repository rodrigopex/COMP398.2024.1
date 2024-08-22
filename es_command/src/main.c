#include <zephyr/kernel.h>
#include <string.h>

/*
 * - Button
 * - Screen
 * - Accelerometer
 */

enum command_type {
	BUTTON,
	SCREEN,
	ACCELEROMETER,
	COMMAND_COUNT,
};

struct command {
	enum command_type type;
	char *name;
	int (*function)(void *ptr);
};

int button_test(void *ptr)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

int screen_test(void *ptr)
{
	printk("%s. Show message: %s\n", __FUNCTION__, (char *)ptr);
	return 0;
}

int accelerometer_test(void *ptr)
{
	printk("%s\n", __FUNCTION__);
	return 0;
}

struct command command_table[] = {{BUTTON, "button_test", button_test},
				  {SCREEN, "screen_test", screen_test},
				  {ACCELEROMETER, "accelerometer_test", accelerometer_test}};

int invoker(enum command_type type, void *arg)
{
	return command_table[type].function(arg);
}

int invoker_string(char name[], void *arg)
{
	for (int i = 0; i < COMMAND_COUNT; ++i) {

		if (strcmp(name, command_table[i].name) == 0) {
			return command_table[i].function(arg);
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	invoker(SCREEN, "Hello World!");
	invoker(BUTTON, NULL);
	invoker(ACCELEROMETER, NULL);

	invoker_string("screen_test", "Hello World!");
	invoker_string("button_test", NULL);
	invoker_string("accelerometer_test", NULL);

	return 0;
}
