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

enum state state_machine(enum event evt)
{
	static enum state state = YELLOW;

	switch (state) {
	case RED:
		if (evt == GO) {
			state = GREEN;
		}
		break;
	case GREEN:
		if (evt == STOP) {
			state = YELLOW;
		}
		break;
	case YELLOW:
		if (evt == TIMEOUT) {
			state = RED;
		}
		break;
	default:
		k_oops();
	}
	return state;
}

int main(int argc, char *argv[])
{
	enum state current_state = STATE_COUNT;
	enum event evts[] = {GO, STOP, GO, TIMEOUT, STOP, GO, STOP, EVENT_COUNT};
	for (int i = 0; evts[i] != EVENT_COUNT; ++i) {
		current_state = state_machine(evts[i]);
	}
	__ASSERT(current_state == YELLOW, "Must be yellow");
	return 0;
}
