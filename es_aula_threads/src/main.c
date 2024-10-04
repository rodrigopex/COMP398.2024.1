#include <stdint.h>
#include <stdbool.h>
#include <zephyr/kernel.h>

#define T1_PRIORITY 3
#define T2_PRIORITY 2

K_MSGQ_DEFINE(msgq_i, sizeof(int), 8, 4);
K_MSGQ_DEFINE(msgq_i2, sizeof(int), 8, 4);

int main(int argc, char *argv[])
{
	printk("main started\n");
	int i = 0;
	char name[] = "main: i";
	while (true) {
		i += 1;
		printk("%s = %d\n", name, i);
		k_msgq_put(&msgq_i, &i, K_SECONDS(1));
		k_msgq_put(&msgq_i2, &i, K_SECONDS(1));
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
	while (1) {
		k_msgq_get(&msgq_i, &i, K_FOREVER);
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
	while (1) {
		k_msgq_get(&msgq_i2, &i, K_FOREVER);
		printk("t2: i = %d\n", i);
	}
}

K_THREAD_DEFINE(t2_tid, 1024, t2, NULL, NULL, NULL, T2_PRIORITY, 0, 0);
