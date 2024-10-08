#include <stdint.h>
#include <stdbool.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/kernel.h>

#define T1_PRIORITY 2
#define T2_PRIORITY 3

ZBUS_CHAN_DEFINE(chan_i, int, NULL, NULL, ZBUS_OBSERVERS(sub_t1, msub_t2), 0);

ZBUS_SUBSCRIBER_DEFINE(sub_t1, 4);

ZBUS_MSG_SUBSCRIBER_DEFINE(msub_t2);

void lis1_cb(const struct zbus_channel *chan)
{
	const int *i = zbus_chan_const_msg(chan);
	printk("Lis1: i = %d\n", *i);
}
ZBUS_LISTENER_DEFINE(lis1, lis1_cb);

ZBUS_CHAN_ADD_OBS(chan_i, lis1, 3);

int main(int argc, char *argv[])
{
	printk("main started\n");
	int i = 0;
	char name[] = "main: i";
	while (true) {
		i += 1;
		printk("%s = %d\n", name, i);
		zbus_chan_pub(&chan_i, &i, K_SECONDS(1));
		// k_msgq_put(&msgq_i, &i, K_SECONDS(1));
		// k_msgq_put(&msgq_i2, &i, K_SECONDS(1));
		k_sleep(K_SECONDS(1));
	}
	return 0;
}

void t1(void *ptr1, void *ptr2, void *ptr3)
{
	ARG_UNUSED(ptr1);
	ARG_UNUSED(ptr2);
	ARG_UNUSED(ptr3);

	printk("t1 started\n");
	int i;

	const struct zbus_channel *chan;

	while (1) {
		zbus_sub_wait(&sub_t1, &chan, K_FOREVER);
		zbus_chan_read(chan, &i, K_FOREVER);
		printk("t1: i = %d\n", i);
	}
}

K_THREAD_DEFINE(t1_tid, 1024, t1, NULL, NULL, NULL, T1_PRIORITY, 0, 0);

void t2(void *ptr1, void *ptr2, void *ptr3)
{
	ARG_UNUSED(ptr1);
	ARG_UNUSED(ptr2);
	ARG_UNUSED(ptr3);

	printk("t2 started\n");
	int i;
	const struct zbus_channel *chan;

	while (1) {
		zbus_sub_wait_msg(&msub_t2, &chan, &i, K_FOREVER);
		// k_msgq_get(&msgq_i2, &i, K_FOREVER);
		printk("t2: i = %d\n", i);
	}
}

K_THREAD_DEFINE(t2_tid, 1024, t2, NULL, NULL, NULL, T2_PRIORITY, 0, 0);
