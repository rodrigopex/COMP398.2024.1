#include <zephyr/kernel.h>

enum event {
	TIMEOUT,
	EVENT_COUNT,
};

enum state {
	RED,
	GREEN,
	YELLOW,
	STATE_COUNT,
};

struct next_state_info {
	enum state next_state;
	k_timeout_t timeout;
};

struct next_state_info state_machine[STATE_COUNT] = {
	[RED] = {.next_state = GREEN, .timeout = K_SECONDS(5)},
	[GREEN] = {.next_state = YELLOW, .timeout = K_SECONDS(1)},
	[YELLOW] = {.next_state = RED, .timeout = K_SECONDS(3)},
};

int state_action(enum state current_state)
{
	switch (current_state) {
	case RED:
		printk("RED\n");
		return 0;
	case GREEN:
		printk("GREEN\n");
		return 0;
	case YELLOW:
		printk("YELLOW\n");
		return 0;
	case STATE_COUNT:
		return -1;
	}
}

enum state current_state = YELLOW;

void timer_state_expiry_fn(struct k_timer *t);
K_TIMER_DEFINE(timer_state, timer_state_expiry_fn, NULL);

void timer_state_expiry_fn(struct k_timer *t)
{
	struct next_state_info new_state_info = state_machine[current_state];

	if (new_state_info.next_state != current_state) {
		state_action(new_state_info.next_state);
		current_state = new_state_info.next_state;
		k_timer_start(&timer_state, new_state_info.timeout, K_NO_WAIT);
	}
}
int main(int argc, char *argv[])
{
	state_action(current_state);
	k_timer_start(&timer_state, state_machine[current_state].timeout, K_NO_WAIT);
	return 0;
}
