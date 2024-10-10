#include <zephyr/kernel.h>

enum event {
	GO,
	STOP,
	TIMEOUT,
	END_OF_EVENTS,
};

enum state {
	RED,
	GREEN,
	YELLOW,
	INVALID,
};

enum state state_machine(enum event evt)
{
	static enum state state = YELLOW;

	switch (evt) {
	case GO:
		if (state == RED) {
			state = GREEN;
		}
		break;
	case STOP:
		if (state == GREEN) {
			state = YELLOW;
		}
		break;
	case TIMEOUT:
		if (state == YELLOW) {
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
	enum state current_state = INVALID;
	enum event evts[] = {GO, STOP, GO, TIMEOUT, STOP, GO, STOP, END_OF_EVENTS};
	for (int i = 0; evts[i] != END_OF_EVENTS; ++i) {
		current_state = state_machine(evts[i]);
	}
	__ASSERT(current_state == YELLOW, "Must be yellow");
	return 0;
}
