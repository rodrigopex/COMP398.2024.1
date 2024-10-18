#include <zephyr/kernel.h>

void t1(struct k_timer *timer)
{
	printk("%s\n", __FUNCTION__);
}
void t2(struct k_timer *timer)
{
	printk("%s\n", __FUNCTION__);
}
void t3(struct k_timer *timer)
{
	printk("%s\n", __FUNCTION__);
}
K_TIMER_DEFINE(timer_sched1, t1, NULL);
K_TIMER_DEFINE(timer_sched2, t2, NULL);
K_TIMER_DEFINE(timer_sched3, t3, NULL);

int main(int argc, char *argv[])
{
	k_timer_start(&timer_sched1, K_SECONDS(1), K_SECONDS(1));
	k_timer_start(&timer_sched2, K_SECONDS(2), K_SECONDS(2));
	k_timer_start(&timer_sched3, K_SECONDS(3), K_SECONDS(3));

	while (1) {
		k_sleep(K_SECONDS(100));
	}
	return 0;
}
