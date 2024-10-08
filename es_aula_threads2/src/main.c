#include <stdint.h>
#include <stdbool.h>
#include <zephyr/zbus/zbus.h>
#include <zephyr/kernel.h>

#define T1_PRIORITY 2
#define T2_PRIORITY 3

K_FIFO_DEFINE(fifo_i);
// K_FIFO_DEFINE(fifo_i2);

int main(int argc, char *argv[])
{
	printk("main started\n");
	int i = 0;
	char name[] = "main: i";
	while (true) {
		i += 1;
		printk("%s = %d\n", name, i);
		k_fifo_alloc_put(&fifo_i, &i);
		// k_fifo_alloc_put(&fifo_i2, &i);
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
	int *i;

	while (1) {
		i = k_fifo_peek_tail(&fifo_i);
		if (i != NULL) {
			printk("t1: i = %d\n", *i);
		}
		k_msleep(10);
	}
}

K_THREAD_DEFINE(t1_tid, 1024, t1, NULL, NULL, NULL, T1_PRIORITY, 0, 0);

void t2(void *ptr1, void *ptr2, void *ptr3)
{
	ARG_UNUSED(ptr1);
	ARG_UNUSED(ptr2);
	ARG_UNUSED(ptr3);

	printk("t2 started\n");
	int *i;

	while (1) {
		i = k_fifo_get(&fifo_i, K_FOREVER);
		printk("t2: i = %d\n", *i);
		k_msleep(1000);
	}
}

K_THREAD_DEFINE(t2_tid, 1024, t2, NULL, NULL, NULL, T2_PRIORITY, 0, 0);
