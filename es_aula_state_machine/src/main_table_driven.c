#include <zephyr/kernel.h>

enum event {
	GO,
	STOP,
	TIMEOUT,
	EVENT_COUNT,
};

enum state {
	RED,
	GREEN,
	YELLOW,
	STATE_COUNT,
};

enum state state_machine[STATE_COUNT][EVENT_COUNT] = {
	[RED] = {[GO] = GREEN, [STOP] = RED, [TIMEOUT] = RED},
	[GREEN] = {[GO] = GREEN, [STOP] = YELLOW, [TIMEOUT] = GREEN},
	[YELLOW] = {[GO] = YELLOW, [STOP] = YELLOW, [TIMEOUT] = RED},
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

int main(int argc, char *argv[])
{
	enum state current_state = YELLOW;
	enum event evts[] = {GO, STOP, GO, TIMEOUT, STOP, GO, STOP, EVENT_COUNT};
	for (int i = 0; evts[i] != EVENT_COUNT; ++i) {
		enum state new_state = state_machine[current_state][evts[i]];
		if (new_state != current_state) {
			state_action(new_state);
			current_state = new_state;
		}
	}
	__ASSERT(current_state == YELLOW, "Must be yellow");
	return 0;
}
